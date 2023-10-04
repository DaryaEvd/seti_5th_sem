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
  if(domain == -1) {
    perror("error in domain");
    pthread_exit(NULL);
  }

  int fileDescrSocket = socket(domain, SOCK_DGRAM, 0);
  if (fileDescrSocket < 0) {
    perror("error in sender socket");
    pthread_exit(NULL);
  }

  struct sockaddr_in6 addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin6_family = (sa_family_t)domain;
  addr.sin6_port = (in_port_t)htons(portNum);

  inet_pton(AF_INET6, groupAddr, &addr.sin6_addr);

  char *messageToSend = "Hello world\n";

  while (1) {
    ssize_t countBytes =
        sendto(fileDescrSocket, messageToSend, strlen(messageToSend),
               0, (struct sockaddr *)&addr, sizeof(addr));
    if (countBytes < 0) {
      perror("error in sender: sendto");
      pthread_exit(NULL);
    }
  }
}
