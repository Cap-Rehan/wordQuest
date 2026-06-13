# WordQuest

A multiplayer terminal word-guessing game over TCP sockets, built in C.

## How to Build

```zsh
make
```

On Windows (MinGW), link Winsock manually:

```bash
gcc server.c game.c -o server -lws2_32
gcc client.c -o client -lws2_32
```

## How to Play

1. Run ./server in one terminal
2. Run ./client in separate terminals (same machine or same WiFi)
3. Each client enters a name
4. Game starts automatically when all players connect

## Game Rules

- One shared word per round
- Players take turns guessing one letter at a time
- Correct guess = +1 point, turn stays
- Wrong guess = turn passes to next player
- Scores carry across all rounds
- A round ends when the word is fully guessed & the game ends when max_rounds are done or any player gets disconnected

## File Structure

server.c — game host, owns all game state
client.c — thin terminal client, display + input only
game.c — game logic (words, guessing, scoring)
game.h — shared structs and function declarations
platform.h — cross-platform socket abstraction (POSIX + Winsock)
words.txt — word bank (word|hint format)
Makefile — build system

## Platform Support

macOS ✅ Linux ✅ Windows ⚠️ (needs MinGW + -lws2_32)

_Last updated: June 2026_
