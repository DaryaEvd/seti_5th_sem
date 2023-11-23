#include "utils.h"
#include "commons.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

// struct sockaddr_in --> "xxx.xxx.xxx.xxx:ppppp"
char *convertAddrToStr(struct sockaddr_in *addr) {
  static char s[22];
  sprintf(s, "%s:%hu", inet_ntoa(addr->sin_addr),
          ntohs(addr->sin_port));
  return s;
}

// in_addr_t --> "xxx.xxx.xxx.xxx"
char *convertToAddr(void *addr) {
  return inet_ntoa(*(struct in_addr *)addr);
}

int startListen(in_addr_t addr, in_port_t port) {
  int currentFileDescr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (currentFileDescr == -1) {
    printf("startListen: socket() err : '%s'\n", strerror(errno));
    return 0;
  }

  struct sockaddr_in bindedAddr;
  memset(&bindedAddr, 0, sizeof(struct sockaddr_in));
  bindedAddr.sin_family = AF_INET;
  bindedAddr.sin_port = port;
  bindedAddr.sin_addr.s_addr = addr;
  printf("binded addr: %s\n", convertAddrToStr(&bindedAddr));
  if (bind(currentFileDescr, (struct sockaddr *)&bindedAddr,
           sizeof(struct sockaddr_in)) == -1) {
    printf("startListen: bind() err : '%s'\n", strerror(errno));
    close(currentFileDescr);
    return 0;
  }
  if (listen(currentFileDescr, SOMAXCONN) == -1) {
    printf("startListen: listen() err : '%s'\n", strerror(errno));
    printf("binded: %s\n", convertAddrToStr(&bindedAddr));
    close(currentFileDescr);
    return 0;
  }
  return currentFileDescr;
}

int startConnection(in_addr_t addr, in_port_t port) {
  int currentFileDescr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (currentFileDescr == -1) {
    printf("startConnection: socket() err : '%s'\n", strerror(errno));
    return 0;
  }

  struct sockaddr_in dstAddr;
  memset(&dstAddr, 0, sizeof(struct sockaddr_in));
  dstAddr.sin_family = AF_INET;
  dstAddr.sin_port = port;
  dstAddr.sin_addr.s_addr = addr;
  printf("Connecting eventTo %s...\n", convertAddrToStr(&dstAddr));

  if (connect(currentFileDescr, (struct sockaddr *)&dstAddr,
              sizeof(struct sockaddr_in)) == -1) {
    printf("startConnection: connect() err : '%s'\n",
           strerror(errno));
    printf("dst: %s\n", convertAddrToStr(&dstAddr));
    close(currentFileDescr);
    return 0;
  }
  printf("Connection established\n");
  return currentFileDescr;
}

// TODO: храни имена
in_addr_t resolveDomain(char *domain) {
  printf("Domain eventTo resolve: %s\n", domain);
  struct hostent *host = gethostbyname(domain);
  if (host == NULL) {
    printf("resolveDomain: gethostbyname() err : '%s'\n",
           strerror(errno));

    printf("Couldn't resolve host '%s'\n", domain);
    return 0;
  }
  printf("RESOLVED domain: %s\n",
         convertToAddr(host->h_addr_list[0]));
  return *(in_addr_t *)host->h_addr_list[0];
}

void getLocalAddr(int currentFileDescr, in_addr_t *addr,
                  in_port_t *port) {
  struct sockaddr_in local_addr;
  socklen_t local_addrlen = sizeof(struct sockaddr_in);
  getsockname(currentFileDescr, (struct sockaddr *)&local_addr,
              &local_addrlen);
  printf("Local Address: %s\n", convertAddrToStr(&local_addr));
  if (addr)
    *addr = local_addr.sin_addr.s_addr;
  if (port)
    *port = local_addr.sin_port;
}

void getPeerAddr(int currentFileDescr, in_addr_t *addr,
                 in_port_t *port) {
  struct sockaddr_in peerAddr;
  socklen_t peer_addrlen = sizeof(struct sockaddr_in);
  getpeername(currentFileDescr, (struct sockaddr *)&peerAddr,
              &peer_addrlen);
  printf("Peer Address: %s\n", convertAddrToStr(&peerAddr));
  if (addr)
    *addr = peerAddr.sin_addr.s_addr;
  if (port)
    *port = peerAddr.sin_port;
}

int doesMethodExist(struct methodRequestStruct *method_req,
                    uint8_t method) {
  for (int i = 0; i < method_req->nmethods; i++)
    if (method_req->methods[i] == method)
      return 1;
  return 0;
}

in_addr_t getDstAddr(union netInfo *dst, uint8_t atyp) {
  if (atyp == IPV4)
    return dst->ipv4.addr;
  char domain[64];
  size_t len = dst->domain.len;
  domain[len] = '\0';
  strncpy(domain, dst->domain.str, len);
  return resolveDomain(domain);
}

in_port_t getDstPort(union netInfo *dst, uint8_t atyp) {
  if (atyp == IPV4)
    return dst->ipv4.port;
  return *(in_port_t *)(dst->domain.str + dst->domain.len);
}

uint8_t *getPayload(struct datagramStruct *dgram, int bufferLength,
                    int *len) {
  uint8_t *payload =
      dgram->atyp == IPV4
          ? (uint8_t *)&dgram->dst.ipv4.port + 2
          : dgram->dst.domain.str + dgram->dst.domain.len + 2;
  *len = bufferLength - (payload - (uint8_t *)dgram);
  return payload;
}
