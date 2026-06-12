# wordQuest

## Status: Under Development ⚠️

This project is still in active development and is **not complete yet**. Features and functionality are subject to change.

## About

**wordQuest** is a multiplayer word-guessing game written in C. It's an IPD submission project that implements a hangman-style game with network communication between a server and multiple clients.

---

## Project Structure & File Descriptions

### Core Game Logic

#### **game.h** - Game Header File
Defines the core data structures and function prototypes for the game:
- **WordEntry**: Structure holding a word and its hint (max 40 chars for word, 160 chars for hint)
- **Player**: Structure for player data including socket, name, and score (max 3 players)
- **GameState**: Main game state containing player info, current word, guessed letters, and game status
- **Key Functions**: Word loading, word selection, player rotation, game initialization, guess processing, and game-over detection

#### **game.c** - Game Logic Implementation
Implements the core game mechanics:
- **load_words()**: Reads words and hints from a file (pipe-delimited format)
- **select_random_word()**: Randomly selects a word from the loaded word list
- **next_player()**: Rotates to the next player in turn
- **initialize_game()**: Sets up the initial game state with players and selected word
- **process_guess()**: Processes a letter guess, returns if it's correct/already guessed
- **is_game_over()**: Checks if all letters in the word have been guessed (handles spaces)
- **main()**: Test implementation that loads words and displays a random word with hint

### Networking

#### **server.c** - Network Server
Implements a TCP server for handling multiple client connections:
- Listens on **PORT 8080**
- Accepts up to 2 client connections (hardcoded limit)
- Receives messages from connected clients
- Sends a greeting response to each client
- Basic socket setup using `AF_INET` (IPv4) and `SOCK_STREAM` (TCP)
- **Note**: Currently a basic implementation without full game logic integration

#### **client.c** - Network Client
Implements a TCP client for connecting to the server:
- Connects to server at **127.0.0.1:8080**
- Sends a greeting message to the server
- Receives and displays the server's response
- Handles socket creation, connection, and communication
- **Note**: Currently a basic implementation without full game logic integration

### Data Files

#### **words.txt** - Word Database
Contains the word list for the game in pipe-delimited format:
```
word|hint
example|A sample word
```
This file is loaded by the game logic to populate available words for guessing.

---

## Current Implementation Status

### ✅ Completed
- Game state data structures
- Core game logic (word selection, guess processing, game-over detection)
- Word file parsing with hints
- Basic socket programming for server and client
- TCP connection establishment

### 🔄 In Progress / TODO
- Full integration of game logic with networking
- Multi-player game flow over sockets
- Message protocol between client and server
- Proper game state synchronization
- Scoring system implementation
- User input handling and game loop
- Error handling and edge cases
- Testing and debugging

---

## Building & Running

**Note**: The project is not yet in a runnable state. Individual components can be compiled separately:

```bash
# Compile game logic test
gcc game.c -o game

# Compile server
gcc server.c -o server

# Compile client
gcc client.c -o client
```

---

*Last updated: June 2026*
