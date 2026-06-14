For Windows (MinGW) users, you need to link Winsock manually. Do not use the default `Makefile`. Instead, run:
```bash
gcc server.c game.c -o server -lws2_32
gcc client.c -o client -lws2_32
```
