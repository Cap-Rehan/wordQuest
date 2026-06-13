#include "platform.h"
#include <stdio.h>
#include <string.h>

#define PORT 8080
#define BUF 1024

/* Returns 1 if the server message is a prompt (expects user input).
   Prompts end with ": " — info messages end with "\n".             */
int is_prompt(const char *buf) {
  int len = strlen(buf);
  return len >= 2 && buf[len - 2] == ':' && buf[len - 1] == ' ';
}

/* Connect to server. Returns socket fd, or exits on failure. */
int connect_to_server(const char *ip, int port) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("socket");
    return -1;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
    printf("Invalid address.\n");
    return -1;
  }
  if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("connect");
    return -1;
  }

  return fd;
}

/* Main game loop: receive from server, print, send if prompted. */
void game_loop(int fd) {
  char buf[BUF];
  char input[64];

  while (1) {
    memset(buf, 0, sizeof(buf));
    int bytes = recv(fd, buf, sizeof(buf) - 1, 0);

    if (bytes <= 0) {
      printf("\nDisconnected from server.\n");
      break;
    }

    buf[bytes] = '\0';
    printf("%s", buf);
    fflush(stdout);

    if (is_prompt(buf)) {
      if (fgets(input, sizeof(input), stdin) == NULL)
        break;
      input[strcspn(input, "\n")] = '\0'; /* strip newline */
      send(fd, input, strlen(input), 0);
    }
  }
}

int main() {
  net_init();
  int fd = connect_to_server("127.0.0.1", PORT);
  if (fd < 0) {
    net_cleanup();
    return 1;
  }

  game_loop(fd);

  close_socket(fd);
  net_cleanup();
  return 0;
}
