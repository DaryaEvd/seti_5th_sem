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
  if (argc != 2) {
    printf("Error! Incorrect input from server!\n");
    printf("Server use: [./name.o] <port number>\n");
    return -1;
  }

  int portNum = atoi(argv[1]);

  char *pathToDirToUploadFiles = "../uploads";

  struct stat st = {0};

  if (stat(pathToDirToUploadFiles, &st) == -1) {
    mkdir(pathToDirToUploadFiles, 0700);
  }

  int socketFileDescr = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFileDescr == -1) {
    perror("server: socket() error");
    return -1;
  }

  int enable = 1;
  if (setsockopt(socketFileDescr, SOL_SOCKET, SO_REUSEADDR, &enable,
                 sizeof(int)) < 0) {
    perror("server: setsockopt(SO_REUSEADDR) error");
    return -1;
  }

  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(atoi(argv[1]));

  // bind
  if (bind(socketFileDescr, (struct sockaddr *)&serverAddr,
           sizeof(serverAddr)) < 0) {
    perror("server: bind() error");
    return -1;
  }

  // struct sockaddr_in clientAddr;

  int maxAmountConnection = 10;
  if (listen(socketFileDescr, maxAmountConnection) < 0) {
    perror("listen() error");
    return -1;
  }

  printf("Server: waiting for clients ...\n");

  while (1) {
    struct sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    socklen_t lengthClientAddr = sizeof(clientAddr);

    int clientAccept =
        accept(socketFileDescr, (struct sockaddr *)&clientAddr,

               &lengthClientAddr);

    if (clientAccept < 0) {
      perror("server: accept() error");
      return -1;
    }

    int portClient = htons(clientAddr.sin_port);
    printf("server: accepted from port #%d\n", portClient);



  }
}
