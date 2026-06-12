#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main() {

  int status, valread, clientFD;
  char *hello = "Hello from Player";
  char buffer[1024] = {0};

  if ((clientFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  char *ip = "127.0.0.1";
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, ip, &serv_addr.sin_addr.s_addr) <= 0) {
    printf("Invalid address / address not supported.\n");
    return -1;
  }

  if ((status = connect(clientFD, (const struct sockaddr *)&serv_addr,
                        sizeof serv_addr)) < 0) {
    printf("Connection failed...\n");
    return -1;
  }

  send(clientFD, (const void *)hello, strlen(hello), 0);
  printf("Message sent to the server.\n");

  valread = recv(clientFD, buffer, sizeof(buffer) - 1, 0);
  printf("%s\n", buffer);

  close(clientFD);

  return 0;
}
