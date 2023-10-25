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

#define MSG_LENGTH 100

#define BYTE 1
#define KILOBYTE 1024
#define MEGABYTE 1024 * 1024
#define GIGABYTE 1024 * 1024 * 1024

#define BUFFER_SIZE 100 * KILOBYTE

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

  long long sizeFileBytes = countSizeFile(file);
  printf("size file: '%lld' Bytes\n", sizeFileBytes);

  double mBytesFile = 0;
  if (sizeFileBytes > MEGABYTE) {
    mBytesFile = (double)sizeFileBytes / KILOBYTE / KILOBYTE;
    int precision = 2;
    printf("size file: '%.*f mBytes \n'", precision, mBytesFile);
  } else {
    printf("size file: '%lf kBytes\n",
           (double)sizeFileBytes / KILOBYTE);
  }

  if (!isValidSizeFile(sizeFileBytes)) {
    printf("client: Size file is too long\n");
    return -1;
  }

  char *extractedFileName = extractLastToken(fullPathToFileToSend);
  if (!isValidFileNameLength(extractedFileName)) {
    printf("Your filename is too long in UTF-8. Rename it or give "
           "another one\n");
    fclose(file);
    return -1;
  }

  printf("file name '%s'\n", extractedFileName);

  char *addressIP = argv[2];
  int portNum = atoi(argv[3]);

  clientInfo *client;
  client = (clientInfo *)malloc(sizeof(clientInfo));
  if (!client) {
    perror("client: malloc() error");
    fclose(file);
    return -1;
  }

  client->socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (client->socketFD == -1) {
    perror("server: socket() error");

    free(client);
    fclose(file);

    return -1;
  }

  int enable = 1;
  if (setsockopt(client->socketFD, SOL_SOCKET, SO_REUSEADDR, &enable,
                 sizeof(int)) < 0) {
    perror("client: setsockopt(SO_REUSEADDR) error");

    close(client->socketFD);
    free(client);
    fclose(file);

    return -1;
  }

  client->address.sin_addr.s_addr = inet_addr(addressIP);
  client->address.sin_family = AF_INET;
  client->address.sin_port = htons(portNum);

  if (connect(client->socketFD, (struct sockaddr *)&client->address,
              sizeof(client->address)) != 0) {
    perror("client: connect() error");

    close(client->socketFD);
    free(client);
    fclose(file);

    return -1;
  }

  printf("client connected\n");

  if (send(client->socketFD, extractedFileName,
           strlen(extractedFileName) + 1, 0) < 0) {
    perror("client: send() fileName error");

    close(client->socketFD);
    free(client);
    fclose(file);

    return -1;
  }

  if (send(client->socketFD, &sizeFileBytes, 8, 0) < 0) {
    perror("client: send() size error ");

    close(client->socketFD);
    free(client);
    fclose(file);

    return -1;
  }

  char buffer[BUFFER_SIZE];
  size_t readBytes = fread(buffer, sizeof(char), BUFFER_SIZE, file);
  while (readBytes > 0) {
    if (send(client->socketFD, buffer, readBytes, 0) < 0) {
      perror("client: error in send() data from file: ");

      close(client->socketFD);
      free(client);
      fclose(file);

      return -1;
    }
    readBytes = fread(buffer, sizeof(char), BUFFER_SIZE, file);
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
