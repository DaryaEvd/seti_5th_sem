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

  struct sockaddr_in addr4;
  struct ip_mreq mreq4;
  struct sockaddr_in6 addr6;
  struct ipv6_mreq mreq6;

  if (domain != AF_INET6) {
    memset(&addr4, 0, sizeof(addr4));
    addr4.sin_family = (sa_family_t)domain;
    addr4.sin_port = (in_port_t)htons(portNum);
    addr4.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fileDescrSocket, (struct sockaddr *)&addr4,
             sizeof(addr4)) < 0) {
      perror("error in listener bind");
      pthread_exit(NULL);
    }

    mreq4.imr_multiaddr.s_addr = inet_addr(groupAddr);
    mreq4.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(fileDescrSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                   &mreq4, sizeof(mreq4)) < 0) {
      perror("error in listener: join multicast");
      pthread_exit(NULL);
    }
  }

  else {
    memset(&addr6, 0, sizeof(addr6));
    addr6.sin6_family = (sa_family_t)domain;
    addr6.sin6_port = (in_port_t)htons(portNum);

    if (bind(fileDescrSocket, (struct sockaddr *)&addr6,
             sizeof(addr6)) < 0) {
      perror("error in listener bind");
      pthread_exit(NULL);
    }

    mreq6.ipv6mr_interface = 0;
    inet_pton(AF_INET6, groupAddr, &mreq6.ipv6mr_multiaddr);

    if (setsockopt(fileDescrSocket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP,
                   &mreq6, sizeof(mreq6)) < 0) {
      perror("error in listener: join multicast");
      pthread_exit(NULL);
    }
  }

  CopyList *copyList = NULL;
  const int buffSize = 1024;

  while (1) {
    char msgbuf[buffSize];

    socklen_t lengthAddr4 = sizeof(addr4);

    socklen_t lengthAddr6 = sizeof(addr6);

    int countBytes;

    if (domain != AF_INET6) {
      countBytes = recvfrom(fileDescrSocket, msgbuf, BUFSIZ, 0,
                            (struct sockaddr *)&addr4, &lengthAddr4);
    } else {
      countBytes = recvfrom(fileDescrSocket, msgbuf, BUFSIZ, 0,
                            (struct sockaddr *)&addr6, &lengthAddr6);
    }

    if (countBytes < 0) {
      perror("error in recvfrom");
      freeList(copyList);
      pthread_exit(NULL);
    }

    const int lengthNewIp = 20;

    Copy copy;
    copy.ip = malloc(sizeof(char) * lengthNewIp);
    if (domain != AF_INET6) {
      inet_ntop(AF_INET, &addr4.sin_addr, copy.ip, lengthNewIp);
    } else {
      inet_ntop(AF_INET6, &addr6.sin6_addr, copy.ip, lengthNewIp);
    }
    copy.currentTime = time(NULL);

    copyList = refreshMembersList(copyList, copy);
  }
}
