#include "addrStuff.h"

#include <arpa/inet.h>

int getDomain(const char *ipAddr) {
  const int bytesCount = 20;
  char networkIpInBytes[bytesCount];

  if(inet_pton(AF_INET, ipAddr, networkIpInBytes) == 1) {
    return AF_INET;
  }
  else if (inet_pton(AF_INET6, ipAddr, networkIpInBytes) == 1) {
    return AF_INET6;
  }

  return -1;
}
