#include "resolver.h"
#include "common.h"

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DOMAIN_CACHE_SIZE 1024

struct domainCacheEntry domainCache[DOMAIN_CACHE_SIZE];

uint32_t countHashDomain(const char *domain) {
  // Simple hash function
  uint32_t hash = 0;
  while (*domain) {
    hash = (hash * 31) + (*domain++);
  }
  return hash % DOMAIN_CACHE_SIZE;
}

void addDomainToCache(const char *domain, in_addr_t addr) {
  uint32_t index = countHashDomain(domain);
  strncpy(domainCache[index].domain, domain,
          sizeof(domainCache[index].domain));
  domainCache[index].addr = addr;
}

in_addr_t lookupDomainInCache(const char *domain) {
  uint32_t index = countHashDomain(domain);
  if (strcmp(domainCache[index].domain, domain) == 0) {
    return domainCache[index].addr;
  }
  return 0;
}

in_addr_t resolveDomain(char *domain) {
  printf("Domain eventTo resolve: %s\n", domain);

  in_addr_t cachedAddr = lookupDomainInCache(domain);
  if (cachedAddr != 0) {
    return cachedAddr;
  }

  struct hostent *host = gethostbyname(domain);
  if (host == NULL) {
    printf("resolveDomain: gethostbyname() err : '%s'\n",
           strerror(errno));

    printf("Couldn't resolve host '%s'\n", domain);
    return 0;
  }
  printf("RESOLVED domain: %s\n",
         convertToAddr(host->h_addr_list[0]));

  in_addr_t addr = *(in_addr_t *)host->h_addr_list[0];
  addDomainToCache(domain, addr);
  return addr;
}
