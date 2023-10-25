#include "stuff.h"

#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// #define SIZE 1 * 8 * 1024
// #define LENGTH_INFO 100

// #define GET_KBYTES 1024

#define MSG_LENGTH 100
#define PATH_LENGTH 4096

#define BYTE 1
#define KILOBYTE 1024
#define MEGABYTE 1024 * 1024
#define GIGABYTE 1024 * 1024 * 1024

#define BUFFER_SIZE 100 * KILOBYTE

pthread_mutex_t mutexAverageSpeed;
pthread_mutex_t mutexSize;
pthread_mutex_t downloadFile;

typedef struct {
  int socketFD;
  struct sockaddr address;
  socklen_t lengthAddr;
} connection_t;

void *connectionFunc(void *arg) {
  connection_t *conn = (connection_t *)arg;

  char fileNameFromClient[MSG_LENGTH];
  memset(fileNameFromClient, '\0', sizeof(fileNameFromClient));

  int clientRecvName = recv(conn->socketFD, fileNameFromClient,
                            sizeof(fileNameFromClient), 0);
  if (clientRecvName < 0) {
    perror("server: recv() fileName error");
    pthread_exit(NULL);
  }

  printf("server: Msg from client (filename): '%s'\n",
         fileNameFromClient);

  long long fileSizeFromClient = 0;

  // pthread_mutex_lock(&mutexSize);
  ssize_t clientRecvSize =
      recv(conn->socketFD, &fileSizeFromClient, 8, 0);
  if (clientRecvSize < 0) {
    perror("server: recv() fileSize error");
    pthread_exit(NULL);
  }
  // pthread_mutex_unlock(&mutexSize);

  printf("server: Msg from client (filesize): '%lf' kBytes \n",
         (double)fileSizeFromClient / KILOBYTE);

  FILE *fileToRecv = NULL;
  char outputFilePath[PATH_LENGTH] = "../build/uploads/";
  if (!strcat(outputFilePath, fileNameFromClient)) {
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
  double speed = 0.0;
  double averageSpeed = 0.0;
  double timeElapsed = 0.0;
  time_t startTime = time(NULL);

  while (receivedBytes < fileSizeFromClient) {
    int readBytes = recv(conn->socketFD, buffer, BUFFER_SIZE, 0);
    if (readBytes < 0) {
      perror("error in recv: ");
      pthread_exit(NULL);
    }
    fwrite(buffer, sizeof(char), readBytes, fileToRecv);

    pthread_mutex_lock(&downloadFile);
    receivedBytes += readBytes;
    pthread_mutex_unlock(&downloadFile);

    speed =
        (double)receivedBytes / KILOBYTE / (time(NULL) - startTime);
    printf("[%s] ,", fileNameFromClient);
    printf("Instantaneous speed: %lf kBytes/sec\n", speed);

    pthread_mutex_lock(&mutexAverageSpeed);
    double averageSpeed =
        (double)receivedBytes / KILOBYTE / (timeElapsed); // + 3.0);
    printf("Average speed per session: %lf kBytes/sec\n",
           averageSpeed);
    pthread_mutex_unlock(&mutexAverageSpeed);

    timeElapsed += 3.0;

    sleep(3);
  }

  char response[MSG_LENGTH];
  memset(response, '\0', sizeof(response));

  if (receivedBytes == fileSizeFromClient) {
    strcpy(response, "sizes are the same");
  } else {
    strcpy(response, "sizes are different");
  }
  ssize_t servSend =
      send(conn->socketFD, response, sizeof(response), 0);
  if (servSend < 0) {
    perror("server: send() in the end error");
    pthread_exit(NULL);
  }
  printf("server: result is: '%s' \n", response);

  // fileSizeFromClient = 0;

  fclose(fileToRecv);
  close(conn->socketFD);
  free(conn);
  return NULL;
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

  int serverSocketFileDescr = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocketFileDescr < 0) {
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

  if (bind(serverSocketFileDescr, (struct sockaddr *)&serverAddr,
           sizeof(serverAddr)) < 0) {
    perror("server: bind() error");
    return -1;
  }

  int maxAmountConnection = 100000;

  if (listen(serverSocketFileDescr, maxAmountConnection) < 0) {
    perror("listen() error");
    return -1;
  }

  printf("Server: waiting for clients ...\n");

  struct sockaddr_in clientAddr;
  memset(&clientAddr, 0, sizeof(clientAddr));

  connection_t *connection;

  connection = (connection_t *)malloc(sizeof(connection_t));
  connection->address.sa_family = AF_INET;
  connection->lengthAddr = sizeof(struct sockaddr_in);

  while ((connection->socketFD =
              accept(serverSocketFileDescr, &connection->address,
                     &connection->lengthAddr))) {

    pthread_t threadID;

    int *newSocket = malloc(sizeof *newSocket);
    *newSocket = connection->socketFD;

    pthread_create(&threadID, 0, connectionFunc, (void *)newSocket);
    pthread_detach(threadID);
  }

  free(connection);

  close(serverSocketFileDescr);

  pthread_exit(NULL);

  return 0;
}
