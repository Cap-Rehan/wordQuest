#define MAX_WORD_LENGTH 40
#define MAX_HINT_LENGTH 160
#define MAX_NAME 20
#define MAX_PLAYERS 2

typedef struct WordEntry {
  char word[MAX_WORD_LENGTH];
  char hint[MAX_HINT_LENGTH];
} WordEntry;

typedef struct Player {
  int socket;
  char name[MAX_NAME];
  int score;
} Player;

typedef struct GameState {
  Player players[MAX_PLAYERS];

  int player_count;
  int current_player_index;
  int round_number;
  WordEntry current_word;
  char guessed_letters[26];

  int game_over;
} GameState;

int load_words(const char *, WordEntry *);

WordEntry select_random_word(WordEntry *, int);

void next_player(GameState *);

void initialize_game(GameState *, WordEntry, struct Player *, int);

int process_guess(GameState *, char);

int is_game_over(const GameState *);
