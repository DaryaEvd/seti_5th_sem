#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 4) {
    printf("Error! Incorrecrt input from client!\n");
    printf("Client use: [./name.o] <path to file> <DNS name or IP "
           "address> "
           "<port number>\n");
    return -1;
  }

  char *pathToFileToSend = argv[1];
  char *address = argv[2];
  int portNum = atoi(argv[3]);

  int clientSocketFileDescr = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocketFileDescr == -1) {
    perror("server: socket() error");
    return -1;
  }

  int enable = 1;
  if (setsockopt(clientSocketFileDescr, SOL_SOCKET, SO_REUSEADDR,
                 &enable, sizeof(int)) < 0) {
    perror("client: setsockopt(SO_REUSEADDR) error");
    return -1;
  }

  struct sockaddr_in serverAddr;
  serverAddr.sin_addr.s_addr = inet_addr(address);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(portNum);

  if (connect(clientSocketFileDescr, (struct sockaddr *)&serverAddr,
              sizeof(serverAddr)) != 0) {
    perror("client: connect() error");
    return -1;
  }

  printf("client connected\n");

  size_t lengthOfFileName = strlen(pathToFileToSend) + 1;

  if (send(clientSocketFileDescr, pathToFileToSend, lengthOfFileName,
           0) < 0) {
    perror("client: send() error");
    return 0;
  }

  char srvMsg[200];
  memset(srvMsg, '\0', sizeof(srvMsg));


  ssize_t cliRecv =
      recv(clientSocketFileDescr, srvMsg, sizeof(srvMsg), 0);
  if (cliRecv < 0) {
    perror("client: recv() error");
    return 0;
  }

  printf("client: server's msg: '%s'\n", srvMsg);

  close(clientSocketFileDescr);

  return 0;
}
