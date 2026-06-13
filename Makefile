CC     = gcc
CFLAGS = -Wall -Wextra

all: server client

server: server.c game.c
	$(CC) $(CFLAGS) server.c game.c -o server

client: client.c
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm -f server client
