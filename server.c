#include "game.h"
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PORT 8080
#define NUM_ROUNDS 2
#define MAX_WORDS 100

// Send Helpers

void send_to_one(int sock, const char *msg) { send(sock, msg, strlen(msg), 0); }

void send_to_all(int *clients, int count, const char *msg) {
  for (int i = 0; i < count; i++) {
    send_to_one(clients[i], msg);
  }
}

// Server Setup

int setup_server(int port) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("socket");
    exit(1);
  }

  /* Without this, re-running the server immediately after closing gives
     "Address already in use". SO_REUSEADDR fixes that during development. */
  int opt = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(1);
  }
  if (listen(fd, MAX_PLAYERS) < 0) {
    perror("listen");
    exit(1);
  }

  printf("Server listening on port %d...\n", port);
  return fd;
}

// Lobby

void run_lobby(int server_fd, int *clients, Player *players, int *count) {
  char buf[256];
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);

  printf("Waiting for %d players...\n", MAX_PLAYERS);

  while (*count < MAX_PLAYERS) {
    int new_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (new_fd < 0) {
      perror("accept");
      continue;
    }

    send_to_one(new_fd, "Enter your name: ");

    int bytes = recv(new_fd, buf, sizeof(buf) - 1, 0);
    if (bytes <= 0) {
      close_socket(new_fd);
      continue;
    }

    buf[bytes] = '\0';
    buf[strcspn(buf, "\r\n")] = '\0'; /* strip newline from nc/telnet */

    clients[*count] = new_fd;
    players[*count].socket = new_fd;
    players[*count].score = 0;
    strncpy(players[*count].name, buf, MAX_NAME - 1);
    players[*count].name[MAX_NAME - 1] = '\0';

    printf("Player %d joined: %s\n", *count + 1, players[*count].name);

    char join_msg[64];
    snprintf(join_msg, sizeof(join_msg), "%s joined! (%d/%d)\n",
             players[*count].name, *count + 1, MAX_PLAYERS);
    send_to_all(clients, *count + 1, join_msg);

    (*count)++;
  }

  send_to_all(clients, *count, "All players connected. Game starting!\n");
}

// Build Masked Word

void build_masked_word(GameState *gs, char *out) {
  const char *word = gs->current_word.word;
  int j = 0;
  for (int i = 0; word[i] != '\0'; i++) {
    if (word[i] == ' ') {
      out[j++] = ' ';
    } else if (strchr(gs->guessed_letters, word[i])) {
      out[j++] = word[i];
    } else {
      out[j++] = '_';
    }
    out[j++] = ' '; // space between each character for readability
  }
  if (j > 0)
    j--; // remove trailing space
  out[j] = '\0';
}

// Build State Message
// One string sent to all players at the start of each turn.

void build_state_msg(GameState *gs, char *buf, int buf_size) {
  char masked[128];
  build_masked_word(gs, masked);

  /* Build scores string: "Alice=2 Bob=0 Carol=1" */
  char scores[128] = "";
  for (int i = 0; i < gs->player_count; i++) {
    char part[32];
    snprintf(part, sizeof(part), "%s=%d ", gs->players[i].name,
             gs->players[i].score);
    strncat(scores, part, sizeof(scores) - strlen(scores) - 1);
  }

  snprintf(buf, buf_size,
           "Word : %s\n"
           "\n"
           "Tried: %s\n"
           "Score: %s\n"
           "\n"
           "Turn : %s\n",
           masked, gs->guessed_letters, scores,
           gs->players[gs->current_player_index].name);
}

// Run Round

int run_round(GameState *gs, int *clients, int count) {
  char state_buf[512];
  char recv_buf[32];

  /* Send round header + hint once at the start */
  char header[256];
  snprintf(header, sizeof(header), "\n--- Round %d ---\n%s\n", gs->round_number,
           gs->current_word.hint);
  send_to_all(clients, count, header);

  while (!is_game_over(gs)) {
    /* Send current state to everyone */
    send_to_all(clients, count,
                "\n------------------------------------------\n");
    build_state_msg(gs, state_buf, sizeof(state_buf));
    send_to_all(clients, count, state_buf);

    int cur = gs->current_player_index;
    send_to_one(clients[cur], "Your guess (one letter): ");

    /* Tell others to wait */
    char wait_msg[64];
    snprintf(wait_msg, sizeof(wait_msg), "Waiting for %s...\n",
             gs->players[cur].name);
    for (int i = 0; i < count; i++) {
      if (i != cur)
        send_to_one(clients[i], wait_msg);
    }

    /* Receive guess from current player */
    int bytes = recv(clients[cur], recv_buf, sizeof(recv_buf) - 1, 0);
    if (bytes <= 0) {
      send_to_all(clients, count, "A player disconnected. Game over.\n");
      return -1;
    }
    recv_buf[bytes] = '\0';
    recv_buf[strcspn(recv_buf, "\r\n")] = '\0';

    char guess = recv_buf[0];
    int result = process_guess(gs, guess);

    char result_msg[64];
    if (result == 1) {
      snprintf(result_msg, sizeof(result_msg), "HIT! '%c' is in the word.\n",
               guess);
      gs->players[cur].score++;
    } else if (result == 0) {
      snprintf(result_msg, sizeof(result_msg),
               "MISS! '%c' is not in the word.\n", guess);
    } else {
      snprintf(result_msg, sizeof(result_msg), "Already tried '%c'.\n", guess);
    }
    send_to_all(clients, count, result_msg);

    if (!is_game_over(gs))
      next_player(gs);
  }

  /* Word guessed — announce it */
  char done_msg[128];
  snprintf(done_msg, sizeof(done_msg), "Word guessed: %s\nNice work %s!\n",
           gs->current_word.word, gs->players[gs->current_player_index].name);
  send_to_all(clients, count, done_msg);

  return 0;
}

int main() {
  net_init();
  srand(time(NULL));

  WordEntry words[MAX_WORDS];
  int word_count = load_words("words.txt", words);
  if (word_count <= 0) {
    fprintf(stderr, "Could not load words.txt\n");
    return 1;
  }

  int server_fd = setup_server(PORT);

  int clients[MAX_PLAYERS];
  Player players[MAX_PLAYERS];
  int player_count = 0;

  run_lobby(server_fd, clients, players, &player_count);

  GameState game;

  for (int r = 0; r < NUM_ROUNDS; r++) {
    WordEntry word = select_random_word(words, word_count);
    initialize_game(&game, word, players, player_count);
    game.round_number = r + 1;
    if (run_round(&game, clients, player_count) < 0)
      break;
    /* Sync scores back so next round starts with correct totals */
    for (int i = 0; i < player_count; i++)
      players[i].score = game.players[i].score;
  }

  /* Final scores */
  char final[256] = "\n=== Final Scores ===\n";
  for (int i = 0; i < player_count; i++) {
    char line[48];
    snprintf(line, sizeof(line), "%s: %d\n", players[i].name, players[i].score);
    strncat(final, line, sizeof(final) - strlen(final) - 1);
  }
  send_to_all(clients, player_count, final);

  for (int i = 0; i < player_count; i++)
    close_socket(clients[i]);
  close_socket(server_fd);
  net_cleanup();
  return 0;
}
