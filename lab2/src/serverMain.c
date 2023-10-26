#include "stuff.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void *connectionFunc(void *arg) {
  int clientSocketInServer = *(int *)arg;

  /*--- receive file name ----*/
  char fileName[MSG_LENGTH];
  memset(fileName, '\0', sizeof(fileName));
  ssize_t getNameStatus =
      recv(clientSocketInServer, fileName, sizeof(fileName), 0);
  if (getNameStatus < 0) {
    perror("server: recv() fileName error");
    pthread_exit(NULL);
  }
  printf("server: Msg from client (filename): '%s'\n", fileName);

  char replyToClient[MSG_LENGTH];
  memset(replyToClient, '\0', sizeof(replyToClient));
  strcpy(replyToClient, "from serv to cli: got file name");
  if (send(clientSocketInServer, replyToClient, sizeof(replyToClient),
           0) < 0) {
    printf("send() error: %d", errno);
    pthread_exit(NULL);
  }
  memset(replyToClient, '\0', sizeof(replyToClient));

  /*--- receive file size ----*/
  __off64_t sizeFromClientInBytes = 0;

  ssize_t getSizeStatus =
      recv(clientSocketInServer, &sizeFromClientInBytes,
           sizeof(sizeFromClientInBytes), 0);
  if (getSizeStatus < 0) {
    perror("recv() error");
    pthread_exit(NULL);
  }

  if (sizeFromClientInBytes < MEGABYTE) {
    printf("server: Msg from client (filesize): '%f' KBytes\n",
           (double)sizeFromClientInBytes / 1024);
  } else {
    printf("server: Msg from client (filesize): '%lf' MBytes\n",
           (double)sizeFromClientInBytes / 1024 / 1024);
  }

  strcpy(replyToClient, "from serv to cli: got size file");
  if (send(clientSocketInServer, replyToClient, sizeof(replyToClient),
           0) < 0) {
    perror("server: recv() fileSize error");
    pthread_exit(NULL);
  }
  memset(replyToClient, '\0', sizeof(replyToClient));

  /*--- work with files' data ----*/
  FILE *fileToRecv = NULL;
  char outputFilePath[500] = "../build/uploads/";
  if (!strcat(outputFilePath, fileName)) {
    printf("server: no mem for strcat\n");
    pthread_exit(NULL);
  }

  if (isExistingFile(outputFilePath) == 1) {
    printf("file '%s' will be deleted and rewrited again\n",
           outputFilePath);
    int statusUnlink = unlink(outputFilePath);
    if (statusUnlink < 0) {
      perror("server: unlink() error");
      pthread_exit(NULL);
    }
    printf("file '%s' has deleted and started writing ... \n",
           outputFilePath);
  }

  fileToRecv = fopen(outputFilePath, "wb+");
  char buffer[BUFFER_SIZE];
  long receivedBytes = 0;

  double timeElapsed = 0.0;
  double currtime = 0;
  double speed = 0.0;
  double averageSpeed = 0.0;

  int wasPrinted = 0;

  time_t startTime = time(NULL);

  while (receivedBytes < sizeFromClientInBytes) {
    int readBytes =
        recv(clientSocketInServer, buffer, BUFFER_SIZE, 0);
    if (readBytes < 0) {
      perror("error in recv: ");
      pthread_exit(NULL);
    }
    ssize_t statFwrite =
        fwrite(buffer, sizeof(char), readBytes, fileToRecv);
    if (statFwrite < 0) {
      perror("error in fwrite(): ");
      pthread_exit(NULL);
    }

    receivedBytes += readBytes;

    if ((currtime = time(NULL) - startTime) >= 3 || !wasPrinted) {
      printf("[%s], ", fileName);

      if (sizeFromClientInBytes < MEGABYTE) {
        speed = (double)receivedBytes; /// currtime;
        printf("Instantaneous speed: %lf Bytes/sec\n", speed);
      } else {
        speed = (double)receivedBytes / 1024 / 1024 / (currtime);
        printf("Instantaneous speed: %lf MBytes/sec\n", speed);
      }

      averageSpeed = speed / 3;
      printf("Average speed per session: %lf MBytes/sec\n",
             averageSpeed);
      startTime = time(NULL);

      wasPrinted = 1;
    }
  }

  char resultOfRecvFile[500];
  memset(resultOfRecvFile, '\0', sizeof(resultOfRecvFile));

  if (receivedBytes == sizeFromClientInBytes) {
    strcpy(resultOfRecvFile, "sizes are the same");
  } else {
    strcpy(resultOfRecvFile, "sizes are different");
  }

  ssize_t servSendStatus =
      send(clientSocketInServer, resultOfRecvFile,
           sizeof(resultOfRecvFile), 0);
  if (servSendStatus < 0) {
    perror("server: send() in the end error");
    pthread_exit(NULL);
  }

  close(clientSocketInServer);
  fclose(fileToRecv);
  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Error! Incorrect input from server!\n");
    printf("Server use: %s <port number>\n", argv[0]);
    return -1;
  }

  int portNum = atoi(argv[1]);

  char *pathToDirToUploadFiles = "../build/uploads";

  int statusCreatingDir = createDir(pathToDirToUploadFiles);
  if (statusCreatingDir != 0) {
    return -1;
  }

  int socketServer = socket(AF_INET, SOCK_STREAM, 0);
  if (socketServer < 0) {
    perror("server: socket() error");
    return -1;
  }

  int enable = 1;
  if (setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR, &enable,
                 sizeof(int)) < 0) {
    perror("server: setsockopt(SO_REUSEADDR) error");
    close(socketServer);
    return 1;
  }

  struct sockaddr_in addrServer;
  memset(&addrServer, 0, sizeof(addrServer));

  addrServer.sin_family = AF_INET;
  addrServer.sin_addr.s_addr = htonl(INADDR_ANY); // don't need to bind a
																								 //	socket to a specific IP
  addrServer.sin_port = htons(portNum);

  if (bind(socketServer, (struct sockaddr *)&addrServer,
           sizeof(addrServer)) < 0) {
    perror("server: bind() error");
    close(socketServer);
    return -1;
  }

  int maxAmountConnection = 100000;

  if (listen(socketServer, maxAmountConnection) < 0) {
    perror("listen() error");
    close(socketServer);
    return -1;
  }

  printf("Server: waiting for clients ...\n");

  while (1) {
    struct sockaddr_in addrClient;
    memset(&addrClient, 0, sizeof(addrClient));

    addrClient.sin_family = AF_INET;
    socklen_t client_size = sizeof(addrClient);

    int clientSocketInServer = accept(
        socketServer, (struct sockaddr *)&addrClient, &client_size);
    if (clientSocketInServer < 0) {
      printf("accept error: %d\n", errno);
      close(socketServer);
      return -1;
    }

    pthread_t threadID;
    pthread_create(&threadID, NULL, connectionFunc,
                   &clientSocketInServer);
  }

  close(socketServer);
  close(socketServer);
  return 0;
}
