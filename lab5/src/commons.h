#ifndef CONSTANT_KEEPER__
#define CONSTANT_KEEPER__

#include <arpa/inet.h>

#define VERSION 0x05

// Method
#define NO_AUTHENTICATION_REQUIRED 0x00
#define NO_ACCEPTABLE_METHODS 0xff

// Status
#define AUTH_SUCCESS 0x00
#define AUTH_FAILURE 0x01

// Address type
#define IPV4 0x01
#define DOMAIN 0x03
#define IPV6 0x04

// Command
#define CONNECT 0x01

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

// Length limits
#define MAX_MethodRequestStruct_LEN 257
#define MIN_MethodRequestStruct_LEN 3
#define MAX_AuthRequestStruct_LEN 513
#define MIN_AuthRequestStruct_LEN 5
#define MAX_SocksRequestStruct_LEN 69
#define MIN_SocksRequestStruct_LEN 10
#define MAX_socksReplyStruct_LEN MAX_SocksRequestStruct_LEN

// Size
#define replyStructMethod_SIZE 2
#define DGRAM_IPV4_SIZE 10
#define socksReplyStruct_SIZE_IPV4 MIN_SocksRequestStruct_LEN

// structures && unions
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

struct methodRequestStruct {
  uint8_t version;
  uint8_t nmethods;
  uint8_t methods[];
};

struct replyStructMethod {
  uint8_t version;
  uint8_t method;
};

struct datagramStruct {
  uint8_t atyp;
  union netInfo dst;
};

struct socksRequestStruct {
  uint8_t version;
  uint8_t cmd;
  uint8_t atyp;
  union netInfo dst;
};

struct socksReplyStruct {
  uint8_t version;
  uint8_t replay;
  uint8_t atyp;
  union netInfo bnd;
};

#endif // CONSTANT_KEEPER__
