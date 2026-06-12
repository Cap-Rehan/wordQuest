#define MAX_WORD_LENGTH 20
#define MAX_HINT 2
#define MAX_NAME 20
#define MAX_PLAYERS 3

typedef struct WordEntry {
  char word[MAX_WORD_LENGTH];
  char hint[MAX_HINT];
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

void initialize_game();
int load_words();
WordEntry select_random_word();
int process_guess();
int is_game_over();
void next_player();
