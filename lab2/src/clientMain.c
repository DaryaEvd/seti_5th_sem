#include "stuff.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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

  __off64_t sizeFileInBytes = countSizeFile(fullPathToFileToSend);

  printf("sizeFile '%ld' bytes\n", sizeFileInBytes);
  printf("sizeFile '%f' kBytes\n", (double)sizeFileInBytes / 1024);
  printf("sizeFile '%lf' mBytes\n",
         (double)sizeFileInBytes / 1024 / 1024);

  if (!isValidSizeFile(sizeFileInBytes)) {
    printf("client: Size file is too long\n");
    return -1;
  }

  char *pureFileName = extractLastToken(fullPathToFileToSend);
  if (!isValidFileNameLength(pureFileName)) {
    printf("Your filename is too long in UTF-8. Rename it or give "
           "another one\n");
    fclose(file);
    return -1;
  }

  char *addressIP = argv[2];
  int portNum = atoi(argv[3]);

  int socketDescr = socket(PF_INET, SOCK_STREAM, 0);
  if (socketDescr < 0) {
    printf("socket failed: %d", errno);
    fclose(file);
    return -1;
  }

  int enable = 1;
  if (setsockopt(socketDescr, SOL_SOCKET, SO_REUSEADDR, &enable,
                 sizeof(int)) < 0) {
    printf("setsockopt(SO_REUSEADDR) failed: %d", errno);
    fclose(file);
    return -1;
  }

  struct sockaddr_in dest;
  dest.sin_family = AF_INET;
  dest.sin_port = htons(portNum);
  dest.sin_addr.s_addr = inet_addr(addressIP);

  if (connect(socketDescr, (struct sockaddr *)&dest, sizeof(dest)) <
      0) {
    fclose(file);
    close(socketDescr);
    return -1;
  }

  printf("client connected\n");

  char replyFromServer[500];
  memset(replyFromServer, '\0', sizeof(replyFromServer));

  if (send(socketDescr, pureFileName, strlen(pureFileName) + 1, 0) <
      0) {
    perror("client: send() fileName error");
    fclose(file);
    close(socketDescr);
    return -1;
  }

  if (recv(socketDescr, replyFromServer, sizeof(replyFromServer), 0) <
      0) {
    perror("recv() error");
    fclose(file);
    close(socketDescr);
    return -1;
  }
  printf("%s\n", replyFromServer);
  memset(replyFromServer, '\0', sizeof(replyFromServer));

  if (send(socketDescr, &sizeFileInBytes, sizeof(sizeFileInBytes),
           0) < 0) {
    perror("client: send() size error ");
    fclose(file);
    close(socketDescr);
    return -1;
  }

  if (recv(socketDescr, replyFromServer, sizeof(replyFromServer), 0) <
      0) {
    perror("recv() error");
    fclose(file);
    close(socketDescr);
    return -1;
  }

  printf("%s\n", replyFromServer);
  memset(replyFromServer, '\0', sizeof(replyFromServer));

  char buffer[BUFFER_SIZE];
  size_t readBytes = fread(buffer, sizeof(char), BUFFER_SIZE, file);
  while (readBytes > 0) {
    if (send(socketDescr, buffer, readBytes, 0) < 0) {
      perror("client: error in send() data from file: ");
      fclose(file);
      close(socketDescr);
      return -1;
    }
    readBytes = fread(buffer, sizeof(char), BUFFER_SIZE, file);
  }

  char resOfSending[500];
  memset(resOfSending, '\0', sizeof(resOfSending));
  if (recv(socketDescr, resOfSending, sizeof(resOfSending), 0) < 0) {
    perror("recv() error\n");
    return 0;
  }
  printf("client: res of transfer file: %s\n", resOfSending);
  memset(resOfSending, '\0', sizeof(resOfSending));

  fclose(file);
  close(socketDescr);
  return 0;
}
