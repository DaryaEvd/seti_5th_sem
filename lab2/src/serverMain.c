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

typedef struct {
  int socketFD;
  struct sockaddr address;
  socklen_t lengthAddr;
} connection_t;

void *connectionFunc(void *arg) {
  connection_t *conn = (connection_t *)arg;

  char fileNameFromClient[200];
  memset(fileNameFromClient, '\0', sizeof(fileNameFromClient));

  int clientRecvName = recv(conn->socketFD, fileNameFromClient,
                            sizeof(fileNameFromClient), 0);
  if (clientRecvName < 0) {
    perror("server: recv() fileName error");
    pthread_exit(NULL);
  }

  printf("server: Msg from client (filename): '%s'\n",
         fileNameFromClient);

  ssize_t fileSizeFromClient;
  // long fileSizeFromClient;
  int clientRecvSize = recv(conn->socketFD, &fileSizeFromClient,
                            sizeof(fileSizeFromClient), 0);
  if (clientRecvSize < 0) {
    perror("server: recv() fileSize error");
    pthread_exit(NULL);
  }

  printf("server: Msg from client (filesize): '%ld' bytes\n",
         fileSizeFromClient);

  FILE *fileToRecv = NULL;

  char output[4096] = "../uploads/";
  if (fileToRecv == NULL) {
    strcat(output, fileNameFromClient);
    fileToRecv = fopen(output, "wb");
  }

  ssize_t sizeOfLittleBuffer = 4096;
  unsigned char clientMessBuffer[sizeOfLittleBuffer];
  ssize_t readedSize = 0;
  ssize_t numBytesRecv = 0;

  // while (1) {
  //   ssize_t bytesToRead = (fileSizeFromClient > sizeOfLittleBuffer)
  //                             ? sizeOfLittleBuffer
  //                             : fileSizeFromClient;
  //   if (bytesToRead < 0) {
  //     break;
  //   }

  //   ssize_t readedAmount = 0;
  //   while (readedAmount != bytesToRead) {
  //     ssize_t count;
  //     if ((count = recv(conn->socketFD, clientMessBuffer +
  //     readedAmount,
  //                       fileSizeFromClient - readedSize, 0)) < 0) {
  //       printf("nerkjnege\n");
  //       pthread_exit(0);
  //     }

  //     readedAmount += count;
  //   }

  //   ssize_t writedAmount = 0;
  //   while (writedAmount != bytesToRead) {
  //     ssize_t count;
  //     if ((count = fwrite(clientMessBuffer, 1, bytesToRead -
  //     writedAmount,
  //                         fileToRecv)) < 0) {
  //       printf("less 0\n");
  //       pthread_exit(0);
  //     }
  //   }
  //   fileSizeFromClient -= bytesToRead;
  // }

  int file_size, bytes_received = 0, total_bytes = 0;
  char buffer[1024];

  // Receive file contents and write to file
  while (total_bytes < file_size) {
    bytes_received =
        recv(conn->socketFD, buffer, 1024, 0);
    fwrite(buffer, 1, bytes_received, fileToRecv);
    total_bytes += bytes_received;

    
  }

  printf("server: path to a file: '%s'\n", output);

  // strcpy(srvMsg, "server servak");

  char isSameSize[200];
  memset(isSameSize, '\0', sizeof(isSameSize));

  printf("readed size is: %ld\n", readedSize);

  if (readedSize == fileSizeFromClient) {
    strcpy(isSameSize, "succesfull downloading");
  } else {
    strcpy(isSameSize, "sizes are different");
    // : %d, %d", readedSize, fileSizeFromClient);
  }

  // const ssize_t fileeLength = readedSize;

  printf("readed size is '%ld', fileSize is '%ld' \n", readedSize,
         fileSizeFromClient);

  int clientSend =
      send(conn->socketFD, isSameSize, strlen(isSameSize), 0);
  if (clientSend < 0) {
    perror("server: send() error");
    return 0;
  }

  // printf("server: already sent this msg: '%s'\n", srvMsg);
  // */

  if (conn->address.sa_family == AF_INET) {
    struct sockaddr_in *sin = (struct sockaddr_in *)conn;
    int portClient = sin->sin_port;
    char buffer[20];
    // inet_ntop(AF_INET, &sin->sin_addr.s_addr, buffer,
    // sizeof(buffer)); printf("server: accepted from port #%d and
    // ip addr '%s'\n",
    //        portClient, buffer);
    printf("server: successfully got file from '%s' addr and '%d' "
           "port",
           inet_ntoa(sin->sin_addr), ntohs(sin->sin_port));
  }

  struct sockaddr_in *sin = (struct sockaddr_in *)conn;
  printf("server: successfully got file from '%s' addr and '%d' port",
         inet_ntoa(sin->sin_addr), ntohs(sin->sin_port));

  close(conn->socketFD);
  free(conn);
  pthread_exit(0);
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

  // bind
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
  // connection->lengthAddr = sizeof(struct sockaddr_in);

  pthread_t thread;

  while (1) {
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
      pthread_create(&thread, 0, connectionFunc, (void *)connection);
      pthread_detach(thread);
    }
  }

  close(serverSocketFileDescr);
  return 0;
}
