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

typedef struct clientInfo {
  int socketFD;
  struct sockaddr_in address;
  char *fileName;
  ssize_t sizeFile;
} clientInfo;

char *extractLastToken(const char *inputPathToFile) {
  int amountSymbolsInLastToken = 0;

  for (int i = strlen(inputPathToFile) - 1; i > 0; i--) {
    if (inputPathToFile[i] == '/') {
      break;
    }
    amountSymbolsInLastToken++;
  }

  char *lastToken =
      calloc(amountSymbolsInLastToken + 1, sizeof(char));

  for (int i = 0; i < amountSymbolsInLastToken; i++) {
    lastToken[i] = inputPathToFile[i + (strlen(inputPathToFile) -
                                        amountSymbolsInLastToken)];
  }

  return lastToken;
}

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
  if (access(fullPathToFileToSend, F_OK) != 0) {
    perror("error in access: ");
    return -1;
  }

  char *extractedFileName = extractLastToken(fullPathToFileToSend);

  if (strlen(extractedFileName) > sizeof(char) * 4096) {
    printf("Your filename is too long. Rename it or give another "
           "one\n");
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
  client->address.sin_family = AF_INET;
  client->address.sin_port = htons(portNum);

  if (connect(client->socketFD, (struct sockaddr *)&client->address,
              sizeof(client->address)) != 0) {

    perror("client: connect() error");
    return -1;
  }

  printf("client connected\n");

  FILE *file = fopen(fullPathToFileToSend, "rb");
  if (file == NULL) {
    free(client);
    perror("reading file error");
    return -1;
  }

  fseek(file, 0L, SEEK_END);
  long sizeFile = ftell(file);
  printf("size file: '%ld' bytes\n", sizeFile);
  rewind(file);

  client->sizeFile = sizeFile;

  if (send(client->socketFD, extractedFileName,
           strlen(extractedFileName) + 1, 0) < 0) {
    perror("client: send() fileName error");
    return -1;
  }

  if (send(client->socketFD, &sizeFile, sizeof(sizeFile), 0) < 0) {
    perror("client: send() size error ");
    return -1;
  }

  int bytes_sent = 0;
  int total_bytes = 0;
  char buffer[1024];
  // Send file contents to server
  while (!feof(file)) {
    bytes_sent = fread(buffer, 1, 1024, file);
    send(client->socketFD, buffer, bytes_sent, 0);
    total_bytes += bytes_sent;
  }

  // const long sizeOfLittleBuffer = 4096;
  // char *littleBuffer = calloc(sizeOfLittleBuffer, sizeof(char));

  // while (1) {
  //   ssize_t bytesToRead = (sizeFile > sizeOfLittleBuffer)
  //                             ? sizeOfLittleBuffer
  //                             : sizeFile;
  //   if (bytesToRead <= 0) {
  //     break;
  //   }
  //   ssize_t readedAmount = 0;
  //   while (readedAmount != bytesToRead) {
  //     ssize_t count;
  //     if ((count = send(client->socketFD, littleBuffer +
  //     readedAmount,
  //                       bytesToRead - readedAmount, 0)) < 0) {
  //       close(client->socketFD);
  //       free(littleBuffer);
  //     }
  //     readedAmount += count;

  //     sizeFile -= bytesToRead;
  //   }
  // }
  // free(littleBuffer);

  // char buff[1024];
  // size_t readedBytes;
  // while((readedBytes = fread(buff, 1, 1024, file)) > 0) {
  //   if(send(client->socketFD, buff, readedBytes, 0) < 0 ) {
  //     perror("client: sending()");
  //     return -1;
  //   }
  // }

  char srvMsg[200];
  memset(srvMsg, '\0', sizeof(srvMsg));

  ssize_t cliRecv = recv(client->socketFD, srvMsg, sizeof(srvMsg), 0);
  if (cliRecv < 0) {
    perror("client: recv() error");
    return 0;
  }

  printf("client: server's msg: '%s'\n", srvMsg);

  close(client->socketFD);

  return 0;
}
