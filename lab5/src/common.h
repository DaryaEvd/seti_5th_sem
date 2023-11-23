#ifndef UTILS__
#define UTILS__

#include <arpa/inet.h>

#define VERSION 0x05

// Method
#define NO_AUTHENTICATION_REQUIRED 0x00
#define NO_ACCEPTABLE_METHODS 0xff

// Command
#define CONNECT 0x01

// Address type
#define IPV4 0x01
#define DOMAIN 0x03
#define IPV6 0x04

// Reply
#define SUCCEEDED 0x00
#define GENERAL_SOCKS_SERVER_FAILURE 0x01
#define CONNECTION_NOT_ALLOWED_BY_RULESET 0x02
#define NETWORK_UNREACHABLE 0x03
#define HOST_UNREACHABLE 0x04
#define CONNECTION_REFUSED 0x05
#define TTL_EXPIRED 0x06
#define COMMAND_NOT_SUPPORTED 0x07
#define ADDRESS_TYPE_NOT_SUPPORTED 0x08

// Length limit
#define MAX_METHOD_REQUEST_LEN 257
#define MIN_METHOD_REQUEST_LEN 3
#define MAX_SOCKS_REQUEST_LEN 69
#define MIN_SOCKS_REQUEST_LEN 10
#define MAX_SOCKS_REPLY_LEN MAX_SOCKS_REQUEST_LEN

// Size
#define METHOD_REPLY_SIZE 2
#define DGRAM_IPV4_SIZE 10
#define SOCKS_REPLY_SIZE_IPV4 MIN_SOCKS_REQUEST_LEN

// version identifier/method selection message
struct methodRequestStruct {
  uint8_t ver;
  uint8_t nmethods;
  uint8_t methods[];
};

// method selection message
struct replyStructMethod {
  uint8_t ver;
  uint8_t method;
};

union netInfo {
  struct {
    in_addr_t addr;
    in_port_t port;
  } ipv4;
  struct {
    uint8_t len;
    uint8_t str[];
  } domain;
  struct {
    uint8_t addr[16];
    in_port_t port;
  } ipv6;
};

struct socksRequestStruct {
  uint8_t ver;
  uint8_t cmd;
  uint8_t rsv;
  uint8_t atyp;
  union netInfo dst;
};

struct socksReplyStruct {
  uint8_t ver;
  uint8_t rep;
  uint8_t rsv;
  uint8_t atyp;
  union netInfo bnd;
};

struct datagramStruct {
  uint16_t rsv;
  uint8_t atyp;
  union netInfo dst;
};

struct domainCacheEntry {
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

int doesMethodExist(struct methodRequestStruct *methodRequest,
                    uint8_t method);
in_addr_t getDstAddr(union netInfo *dst, uint8_t atyp);
in_port_t getDstPort(union netInfo *dst, uint8_t atyp);
uint8_t *getPayload(struct datagramStruct *dgram, int bufferLength,
                    int *len);

#endif
