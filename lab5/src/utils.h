
#ifndef UTILS__
#define UTILS__

#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include "commons.h"


struct domain_cache_entry {
    char domain[64];
    in_addr_t addr;
};


char *convertAddrToStr(struct sockaddr_in *addr);
char *convertToAddr(void *addr);
int startListen(in_addr_t addr, in_port_t port);
int startConnection(in_addr_t addr, in_port_t port);
in_addr_t resolveDomain(char *domain);
void getLocalAddr(int currentFileDescr, in_addr_t *addr,
                  in_port_t *port);
void getPeerAddr(int currentFileDescr, in_addr_t *addr,
                 in_port_t *port);

int doesMethodExist(struct methodRequestStruct *method_req,
                    uint8_t method);
in_addr_t getDstAddr(union netInfo *dst, uint8_t atyp);
in_port_t getDstPort(union netInfo *dst, uint8_t atyp);
uint8_t *getPayload(struct datagramStruct *dgram, int bufferLength,
                    int *len);

#endif // UTILS__
