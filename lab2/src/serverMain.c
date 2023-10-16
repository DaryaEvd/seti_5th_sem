#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void *connectionFunc(void *socketFD) {
  int startSocket = *(int *)socketFD;

  char srvMsg[200];
  memset(srvMsg, '\0', sizeof(srvMsg));

  char cliMsg[200];
  memset(cliMsg, '\0', sizeof(cliMsg));

  int clientRecv = recv(startSocket, cliMsg, sizeof(cliMsg), 0);
  if (clientRecv < 0) {
    perror("server: recv() error");
    // return -1;
    pthread_exit(NULL);
  }

  printf("server: Msg from client: '%s'\n", cliMsg);

  strcpy(srvMsg, "server servak");

  int clientSend = send(startSocket, srvMsg, strlen(srvMsg), 0);
  if (clientSend < 0) {
    perror("server: send() error");
    return 0;
  }

  printf("server: already sent this msg: '%s'\n", srvMsg);

  return NULL;
}

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

  int serverSocketFileDescr = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocketFileDescr == -1) {
    perror("server: socket() error");
    return -1;
  }

  int enable = 1;
  if (setsockopt(serverSocketFileDescr, SOL_SOCKET, SO_REUSEADDR,
                 &enable, sizeof(int)) < 0) {
    perror("server: setsockopt(SO_REUSEADDR) error");
    return -1;
  }

  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(portNum);

  // bind
  if (bind(serverSocketFileDescr, (struct sockaddr *)&serverAddr,
           sizeof(serverAddr)) < 0) {
    perror("server: bind() error");
    return -1;
  }

  // struct sockaddr_in clientAddr;

  int maxAmountConnection = 10;
  if (listen(serverSocketFileDescr, maxAmountConnection) < 0) {
    perror("listen() error");
    return -1;
  }

  printf("Server: waiting for clients ...\n");

  struct sockaddr_in clientAddr;
  memset(&clientAddr, 0, sizeof(clientAddr)); //?????

  clientAddr.sin_family = AF_INET;
  socklen_t lengthClientAddr = sizeof(clientAddr);

  while (1) {
    int clientAccept =
        accept(serverSocketFileDescr, (struct sockaddr *)&clientAddr,
               &lengthClientAddr);
    if (clientAccept < 0) {
      perror("server: accept() error");
      return -1;
    }

    pthread_t thread;
    int *newSocket = malloc(sizeof *newSocket);
    *newSocket = clientAccept;

    int statusCreatingThread = pthread_create(
        &thread, NULL, connectionFunc, (void *)newSocket);
    if (statusCreatingThread < 0) {
      perror("pthread_create() error");
      return -1;
    }

    int portClient = htons(clientAddr.sin_port);
    printf("server: accepted from port #%d\n", portClient);

    /*
    char srvMsg[200];
    memset(srvMsg, '\0', sizeof(srvMsg));

    char cliMsg[200];
    memset(cliMsg, '\0', sizeof(cliMsg));

    int clientRecv = recv(clientAccept, cliMsg, sizeof(srvMsg), 0);
    if (clientRecv < 0) {
      perror("server: recv() error");
      return -1;
    }

    printf("server: Msg from client: '%s'\n", cliMsg);

    strcpy(srvMsg, "server servak");

    int clientSend = send(clientAccept, srvMsg, strlen(srvMsg), 0);
    if (clientSend < 0) {
      perror("server: send() error");
      return 0;
    }

    printf("server: already sent this msg: '%s'\n", srvMsg);

    close(clientAccept);
    */
   
  }

  close(serverSocketFileDescr);
  return 0;
}
