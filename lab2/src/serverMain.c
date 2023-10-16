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

  char srvMsg[200];
  memset(srvMsg, '\0', sizeof(srvMsg));

  char cliMsg[200];
  memset(cliMsg, '\0', sizeof(cliMsg));

  int clientRecv = recv(conn->socketFD, cliMsg, sizeof(cliMsg), 0);
  if (clientRecv < 0) {
    perror("server: recv() error");
    pthread_exit(NULL);
  }

  printf("server: Msg from client: '%s'\n", cliMsg);

  strcpy(srvMsg, "server servak");

  int clientSend = send(conn->socketFD, srvMsg, strlen(srvMsg), 0);
  if (clientSend < 0) {
    perror("server: send() error");
    return 0;
  }

  printf("server: already sent this msg: '%s'\n", srvMsg);

  if (conn->address.sa_family == AF_INET) {
    struct sockaddr_in *sin = (struct sockaddr_in *)conn;
    int portClient = sin->sin_port;
    char buffer[20];
    inet_ntop(AF_INET, &sin->sin_addr.s_addr, buffer, sizeof(buffer));
    printf("server: accepted from port #%d and ip addr '%s'\n",
           portClient, buffer);
  }

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
