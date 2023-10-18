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

#define SIZE 4096 * 4
#define LENGTH_INFO 100
#define PATH_LENGTH 4096
#define GET_KBYTES 1024

pthread_mutex_t mutexAverageSpeed;
pthread_mutex_t mutexSize;

typedef struct {
  int socketFD;
  struct sockaddr address;
  socklen_t lengthAddr;
} connection_t;

void *connectionFunc(void *arg) {
  connection_t *conn = (connection_t *)arg;

  char fileNameFromClient[LENGTH_INFO];
  memset(fileNameFromClient, '\0', sizeof(fileNameFromClient));

  int clientRecvName = recv(conn->socketFD, fileNameFromClient,
                            sizeof(fileNameFromClient), 0);
  if (clientRecvName < 0) {
    perror("server: recv() fileName error");
    pthread_exit(NULL);
  }

  printf("server: Msg from client (filename): '%s'\n",
         fileNameFromClient);

  pthread_mutex_lock(&mutexSize);
  long fileSizeFromClient = 0;

  int clientRecvSize = recv(conn->socketFD, &fileSizeFromClient,
                            sizeof(fileSizeFromClient), 0);
  if (clientRecvSize < 0) {
    perror("server: recv() fileSize error");
    pthread_exit(NULL);
  }

  printf("server: Msg from client (filesize): '%ld' bytes\n",
         fileSizeFromClient);
  pthread_mutex_unlock(&mutexSize);

  FILE *fileToRecv = NULL;
  char outputFilePath[PATH_LENGTH] = "../uploads/";
  strcat(outputFilePath, fileNameFromClient);
  fileToRecv = fopen(outputFilePath, "wb+");

  char buffer[SIZE];
  long receivedBytes = 0;
  double speed = 0.0;
  double averageSpeed = 0.0;
  double timeElapsed = 0.0;
  time_t startTime = time(NULL);

  while (receivedBytes < fileSizeFromClient) {
    int readBytes = recv(conn->socketFD, buffer, SIZE, 0);
    if (readBytes < 0) {
      perror("error in recv: ");
      return NULL;
    }
    fwrite(buffer, sizeof(char), readBytes, fileToRecv);

    receivedBytes += readBytes;
    speed =
        (double)receivedBytes / GET_KBYTES / (time(NULL) - startTime);
    printf("[%s] ,", fileNameFromClient);
    printf("Instantaneous speed: %lf kBytes/sec\n", speed);

    pthread_mutex_lock(&mutexAverageSpeed);
    double averageSpeed =
        (double)receivedBytes / GET_KBYTES / (timeElapsed + 3.0);
    printf("Average speed per session: %lf kBytes/sec\n",
           averageSpeed);
    pthread_mutex_unlock(&mutexAverageSpeed);

    timeElapsed += 3.0;

    sleep(3);
  }

  char response[LENGTH_INFO];
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

  char *pathToDirToUploadFiles = "../uploads";
  struct stat st = {0};
  if (stat(pathToDirToUploadFiles, &st) == -1) {
    printf("server: creating an 'uploads' folder\n");
    mkdir(pathToDirToUploadFiles, 0700);
  } else {
    printf("server: 'uploads' folder already exists\n");
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

  int maxAmountConnection = 4;
  if (listen(serverSocketFileDescr, maxAmountConnection) < 0) {
    perror("listen() error");
    return -1;
  }

  printf("Server: waiting for clients ...\n");

  struct sockaddr_in clientAddr;
  memset(&clientAddr, 0, sizeof(clientAddr));

  connection_t *connection;

  pthread_t threadID[maxAmountConnection];

  for (size_t i = 0; i < maxAmountConnection; i++) {
    connection = (connection_t *)malloc(sizeof(connection_t));
    connection->address.sa_family = AF_INET;
    connection->socketFD =
        accept(serverSocketFileDescr, &connection->address,
               &connection->lengthAddr);
    if (connection->socketFD < 0) {
      free(connection);
      perror("server: accept() error");
      return -1;
    } else {
      pthread_create(&threadID[i], 0, connectionFunc,
                     (void *)connection);
    }
  }

  for (size_t j = 0; j < maxAmountConnection; j++) {
    pthread_join(threadID[j], NULL);
  }

  free(connection);
  close(serverSocketFileDescr);
  return 0;
}
