#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main() {

  char *msg = "Hello from Pankaj";
  char buffer[1024] = {0};
  ssize_t valread;

  int socketFD = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(socketFD, (const struct sockaddr *)&address, sizeof(address)) < 0) {
    printf("Bind failed.\n");
    return -1;
  }

  if (listen(socketFD, 5) < 0) {
    printf("Error listening...\n");
    return -1;
  }

  int clients[10];
  int playerCount = 0;

  socklen_t addrLen = sizeof(address);

  while (1) {
    int newSocket = accept(socketFD, (struct sockaddr *)&address, &addrLen);

    if (newSocket < 0) {
      printf("couldn't connect a client.");
      continue;
    }
    clients[playerCount] = newSocket;
    playerCount++;
    if (playerCount == 2) {
      break;
    }
  }

  for (int i = 0; i < playerCount; i++) {

    valread = recv(clients[i], buffer, sizeof(buffer) - 1, 0);
    if (valread > 0) {
      buffer[valread] = '\0';
    }

    printf("Message from Client %d:\n%s\n\n", i + 1, buffer);

    send(clients[i], (const void *)msg, strlen(msg), 0);
    printf("Hello sent to Client %d\n\n", i + 1);

    close(clients[i]);
  }

  close(socketFD);

  return 0;
}
