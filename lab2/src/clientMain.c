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

#include "stuff.h"

#define SIZE 1 * 8 * 1024
#define MSG_LENGTH 100
#define CLIENT_LENGTH 4096

typedef struct ClientInfo {
  int socketFD;
  struct sockaddr_in address;
} clientInfo;

int main(int argc, char **argv) {
  if (argc != 4) {
    printf("Error! Incorrecrt input from client!\n");
    printf("Client use: %s <path to file> <DNS name or IP "
           "address> "
           "<port number>\n",
           argv[0]);
    return -1;
  }

  char *fullPathToFileToSend = argv[1];
  if (!isExistingFile(fullPathToFileToSend)) {
    printf("Such file by this path '%s' doesn't exists. Try to enter "
           "another file!\n",
           fullPathToFileToSend);
    return -1;
  }

  FILE *file = fopen(fullPathToFileToSend, "rb");
  if (file == NULL) {
    perror("reading file error");
    return -1;
  }

  unsigned long sizeFile = countSizeFile(file);
  double mBytesFile = (double)sizeFile / 1024 / 1024;
  int precision = 2;
  printf("size file: '%.*f mBytes \n'", precision, mBytesFile);

  char *extractedFileName = extractLastToken(fullPathToFileToSend);
  if (!isValidFileNameLength(extractedFileName)) {
    printf("Your filename is too long in UTF-8. Rename it or give "
           "another one\n");
    return -1;
  }

  printf("file name '%s'\n", extractedFileName);

  char *addressIP = argv[2];
  int portNum = atoi(argv[3]);

  clientInfo *client;
  client = (clientInfo *)malloc(sizeof(clientInfo));

  client->socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (client->socketFD == -1) {
    perror("server: socket() error");
    return -1;
  }

  int enable = 1;
  if (setsockopt(client->socketFD, SOL_SOCKET, SO_REUSEADDR, &enable,
                 sizeof(int)) < 0) {
    perror("client: setsockopt(SO_REUSEADDR) error");
    return -1;
  }

  client->address.sin_addr.s_addr = inet_addr(addressIP);
  client->address.sin_family = AF_INET; // TODO: why not ipv6?
  client->address.sin_port = htons(portNum);

  if (connect(client->socketFD, (struct sockaddr *)&client->address,
              sizeof(client->address)) != 0) {
    perror("client: connect() error");
    return -1;
  }

  printf("client connected\n");

  if (send(client->socketFD, extractedFileName,
           strlen(extractedFileName) + 1, 0) < 0) {
    perror("client: send() fileName error");
    return -1;
  }

  if (send(client->socketFD, &sizeFile, sizeof(sizeFile), 0) < 0) {
    perror("client: send() size error ");
    return -1;
  }

  char buffer[SIZE];
  int readBytes = fread(buffer, sizeof(char), SIZE, file);
  while (readBytes > 0) {
    if (send(client->socketFD, buffer, readBytes, 0) < 0) {
      perror("error in send: ");
      return -1;
    }
    readBytes = fread(buffer, sizeof(char), SIZE, file);
  }

  char msgFromServer[MSG_LENGTH];
  memset(msgFromServer, '\0', sizeof(msgFromServer));
  ssize_t cliRecv =
      recv(client->socketFD, msgFromServer, sizeof(msgFromServer), 0);
  if (cliRecv < 0) {
    close(client->socketFD);
    free(client);
    fclose(file);
    free(extractedFileName);

    perror("client: recv() error");
    return 0;
  }

  printf("client: server's msg: '%s'\n", msgFromServer);

  close(client->socketFD);
  free(client);
  fclose(file);
  free(extractedFileName);

  return 0;
}
