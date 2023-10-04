#include "sender.h"
#include "addrStuff.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void *sendThreadWorks(void *arg) {
  char **argv = (char **)arg;

  char *groupAddr = argv[1];
  const int portNum = atoi(argv[2]);

  int domain = getDomain(groupAddr);

  int fileDescrSocket = socket(domain, SOCK_DGRAM, 0);
  if (fileDescrSocket < 0) {
    perror("error in sender socket");
    pthread_exit(NULL);
  }

  struct sockaddr_in addr4;
  if (domain != AF_INET6) {
    memset(&addr4, 0, sizeof(addr4));
    addr4.sin_family = (sa_family_t)domain;
    addr4.sin_port = (in_port_t)htons(portNum);
    addr4.sin_addr.s_addr = inet_addr(groupAddr);
  }

  struct sockaddr_in6 addr6;
  memset(&addr6, 0, sizeof(addr6));
  addr6.sin6_family = (sa_family_t)domain;
  addr6.sin6_port = (in_port_t)htons(portNum);
  inet_pton(AF_INET6, groupAddr, &addr6.sin6_addr);

  char *messageToSend = "Hello world\n";

  while (1) {
    ssize_t countBytes;

    if (domain != AF_INET6) {
      countBytes = sendto(fileDescrSocket, messageToSend,
                          strlen(messageToSend), 0,
                          (struct sockaddr *)&addr4, sizeof(addr4));
    } else {
      countBytes = sendto(fileDescrSocket, messageToSend,
                          strlen(messageToSend), 0,
                          (struct sockaddr *)&addr6, sizeof(addr6));
    }

    if (countBytes < 0) {
      perror("error in sender: sendto");
      pthread_exit(NULL);
    }
  }
}
