#include "listener.h"
#include "addrStuff.h"
#include "copyList.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

void *listenThreadWorks(void *arg) {
  char **argv = (char **)arg;

  char *groupAddr = argv[1];
  const int portNum = atoi(argv[2]);

  int domain = getDomain(groupAddr);

  int fileDescrSocket = socket(domain, SOCK_DGRAM, 0);
  if (fileDescrSocket < 0) {
    perror("error in listener socket");
    pthread_exit(NULL);
  }

  int enableReusing = 1;
  if (setsockopt(fileDescrSocket, SOL_SOCKET, SO_REUSEADDR,
                 &enableReusing, sizeof(enableReusing)) < 0) {
    perror("error in listener enable reusing addr");
    pthread_exit(NULL);
  }

  struct sockaddr_in6 addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin6_family = (sa_family_t)domain;
  addr.sin6_port = (in_port_t)htons(portNum);
  // addr.sin6_addr.s_addr = htonl(INADDR_ANY);

  if (bind(fileDescrSocket, (struct sockaddr *)&addr, sizeof(addr)) <
      0) {
    perror("error in listener bind");
    pthread_exit(NULL);
  }

  struct ipv6_mreq mreq;
  mreq.ipv6mr_interface = 0;
  inet_pton(AF_INET6, groupAddr, &mreq.ipv6mr_multiaddr);

  if (setsockopt(fileDescrSocket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP,
                 &mreq, sizeof(mreq)) < 0) {
    perror("error in listener: join multicast");
    pthread_exit(NULL);
  }

  CopyList *copyList = NULL;

  const int buffSize = 1024;

  while (1) {
    char msgbuf[buffSize];
    socklen_t lengthAddr = sizeof(addr);
    int countBytes = recvfrom(fileDescrSocket, msgbuf, BUFSIZ, 0,
                              (struct sockaddr *)&addr, &lengthAddr);
    if (countBytes < 0) {
      perror("error in recvfrom");
      freeList(copyList);
      pthread_exit(NULL);
    }

    const int lengthNewIp = 20;

    Copy copy;
    copy.ip = malloc(sizeof(char) * lengthNewIp);
    inet_ntop(AF_INET6, &addr.sin6_addr, copy.ip, lengthNewIp);
    copy.currentTime = time(NULL);

    copyList = refreshMembersList(copyList, copy);
  }
}
