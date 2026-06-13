#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int load_words(const char *filename, WordEntry *words) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error while opening the file");
    return -1;
  }

  int i = 0;
  char line[200];
  char word[40];
  char hint[160];

  while (fgets(line, sizeof(line), fp)) {
    sscanf(line, "%[^|]|%[^\n]", word, hint);
    strcpy(words[i].word, word);
    strcpy(words[i].hint, hint);
    i++;
  }

  fclose(fp);
  return i;
}

WordEntry select_random_word(WordEntry *words, int wordCount) {
  int random = 0 + rand() % wordCount;
  return words[random];
}

void next_player(GameState *gstate) {
  gstate->current_player_index++;
  if (gstate->current_player_index >= gstate->player_count) {
    gstate->current_player_index = 0;
  }
}

void initialize_game(GameState *gs, WordEntry theWord, struct Player *players,
                     int player_count) {
  gs->current_word = theWord;
  gs->round_number = 1;

  gs->player_count = player_count;
  for (int i = 0; i < player_count; i++) {
    gs->players[i] = players[i];
  }

  gs->current_player_index = 0;
  gs->game_over = 0;
  memset(gs->guessed_letters, 0, sizeof(gs->guessed_letters));
}

int process_guess(GameState *gs, char guess) {
  int found = 0;
  for (int i = 0; i < (int)sizeof gs->guessed_letters; i++) {
    if (gs->guessed_letters[i] == guess) {
      found = 1;
      break;
    }
  }
  if (found)
    return 2;

  int len = strlen(gs->guessed_letters);
  gs->guessed_letters[len] = guess;
  gs->guessed_letters[len + 1] = '\0';

  for (int i = 0; i < (int)strlen(gs->current_word.word); i++) {
    if (gs->current_word.word[i] == guess)
      return 1;
  }
  return 0;
}

int is_game_over(const GameState *gs) {
  const char *word = gs->current_word.word;

  for (int i = 0; word[i] != '\0'; i++) {
    if (word[i] == ' ') {
      continue;
    }

    if (strchr(gs->guessed_letters, word[i]) == NULL) {
      return 0;
    }
  }
  return 1;
}
