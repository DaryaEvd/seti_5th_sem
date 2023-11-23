#include "handlerTCP.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

void runTcpCallback(struct eventData *fdData,
                    struct eventState *state) {
  uint8_t buf[BUFFER_SIZE];
  int buflen = recv(fdData->currentFileDescr, buf, BUFFER_SIZE, 0);
  if (buflen == -1 && errno != ECONNRESET) {
    printf("runTcpCallback: recv() err : '%s'\n", strerror(errno));
  } else if (buflen > 0) {
    buflen = send(fdData->eventTo->currentFileDescr, buf, buflen, 0);
    if (buflen > 0) {
      return;
    } else if (errno != ECONNRESET) {
      printf("runTcpCallback: send() err : '%s'\n", strerror(errno));
    }
  }
  printf("TCP relay stopped\n");
  stopCallback(fdData, state);
}

void requestCallback(struct eventData *fdData,
                     struct eventState *state) {
  printf("client: %s\n", convertAddrToStr(fdData->addr));
  int reject;
  uint8_t recvDataFromClient[MAX_SOCKS_REQUEST_LEN] = {0};
  uint8_t clientReplay[MAX_SOCKS_REPLY_LEN] = {0};
  struct socksRequestStruct *req =
      (struct socksRequestStruct *)recvDataFromClient;
  struct socksReplyStruct *rep =
      (struct socksReplyStruct *)clientReplay;
  rep->ver = VERSION;

  reject = 1;
  int buflen = recv(fdData->currentFileDescr, recvDataFromClient,
                    MAX_SOCKS_REQUEST_LEN, 0);
  if (buflen > 0) {

    if (buflen < MIN_SOCKS_REQUEST_LEN) {
      printf("Invalid SOCKS request\n");
    } else {
      reject = 0;
    }
  } else if (buflen == -1 && errno != ECONNRESET) {
    printf("requestCallback: recv() err : '%s'\n", strerror(errno));

    printf("client: %s\n", convertAddrToStr(fdData->addr));
  }

  if (reject) {
    clearEvent(fdData, state);
    closeAndFree(fdData);
    printf("Connection closed\n");
    return;
  }

  reject = 1;
  rep->rep = SUCCEEDED;
  if (req->atyp == IPV6) {
    rep->rep = ADDRESS_TYPE_NOT_SUPPORTED;
    clearEvent(fdData, state);
    closeAndFree(fdData);
    printf("Unfortunately, address type is not supported\n");
  } else {
    if (req->cmd == CONNECT) {
      in_addr_t addr = getDstAddr(&req->dst, req->atyp);
      in_port_t port = getDstPort(&req->dst, req->atyp);
      int dstFD = startConnection(addr, port);
      if (dstFD) {
        fdData->callback = runTcpCallback;
        fdData->eventTo = eventSet(dstFD, runTcpCallback, state);
        fdData->eventTo->eventTo = fdData;

        fdData->eventTo->addr = malloc(sizeof(struct sockaddr_in));
        if (!fdData->eventTo->addr) {
          printf("requestCallback: malloc() err : '%s'\n",
                 strerror(errno));
          exit(EXIT_FAILURE);
        }
        fdData->eventTo->addr->sin_port = port;
        fdData->eventTo->addr->sin_addr.s_addr = addr;

        rep->atyp = IPV4;
        getLocalAddr(dstFD, &rep->bnd.ipv4.addr, &rep->bnd.ipv4.port);
        printf("rep.bnd: %s:%hu\n",
               convertToAddr(&rep->bnd.ipv4.addr),
               ntohs(rep->bnd.ipv4.port));
        reject = 0;
      } else {
        rep->rep = GENERAL_SOCKS_SERVER_FAILURE;
        printf("client: %s\n", convertAddrToStr(fdData->addr));
      }
    }

    else {
      rep->rep = COMMAND_NOT_SUPPORTED;
      printf("Command not supported\n\n");
    }
  }

  if (send(fdData->currentFileDescr, clientReplay,
           SOCKS_REPLY_SIZE_IPV4, 0) > 0) {
    printf("SOCKS Reply sent successfully\n\n");
  } else {
    reject = 1;
    printf("requestCallback: send() err : '%s'\n", strerror(errno));
    printf("client: %s\n", convertAddrToStr(fdData->addr));
  }

  if (reject) {
    if (fdData->eventTo) {
      clearEvent(fdData->eventTo, state);
      closeAndFree(fdData->eventTo);
      printf("Disconnect from destination\n");
    }
    clearEvent(fdData, state);
    closeAndFree(fdData);
    printf("Connection closed\n");
  }
}

void handshakeCallback(struct eventData *fdData,
                       struct eventState *state) {
  printf("client: %s\n", convertAddrToStr(fdData->addr));
  int reject;
  uint8_t recvDataFromClient[MAX_METHOD_REQUEST_LEN] = {0},
          clientReplay[METHOD_REPLY_SIZE] = {0};
  struct methodRequestStruct *req =
      (struct methodRequestStruct *)recvDataFromClient;
  struct replyStructMethod *rep =
      (struct replyStructMethod *)clientReplay;
  rep->ver = VERSION;

  reject = 1;
  int buflen = recv(fdData->currentFileDescr, recvDataFromClient,
                    MAX_METHOD_REQUEST_LEN, 0);
  if (buflen > 0) {

    if (buflen < MIN_METHOD_REQUEST_LEN) {
      printf("Invalid version identifier/method selection message\n");
    } else {
      reject = 0;
    }
  } else if (buflen == -1 && errno != ECONNRESET) {
    printf("handshakeCallback: recv() err : '%s'\n", strerror(errno));

    printf("client: %s\n", convertAddrToStr(fdData->addr));
  }

  if (reject) {
    clearEvent(fdData, state);
    closeAndFree(fdData);
    printf("Connection closed\n");
    return;
  }

  // method request
  reject = 1;
  rep->method = NO_ACCEPTABLE_METHODS;
  if ((int)req->ver == VERSION) {
    if (doesMethodExist(req, NO_AUTHENTICATION_REQUIRED)) {
      rep->method = NO_AUTHENTICATION_REQUIRED;
      fdData->callback = requestCallback;
      reject = 0;
    } else {
      printf("No acceptable methods\n");
    }
  } else {
    printf("Protocol version is not supported\n");
  }

  // method reply
  if (send(fdData->currentFileDescr, clientReplay, METHOD_REPLY_SIZE,
           0) > 0) {
    printf("METHOD selection message sent successfully\n");
  } else {
    reject = 1;
    printf("handshakeCallback: send() err : '%s'\n", strerror(errno));
    printf("client: %s\n", convertAddrToStr(fdData->addr));
  }

  if (reject) {
    clearEvent(fdData, state);
    closeAndFree(fdData);
    printf("Connection closed\n");
  }
}

int setNonBlocking(int sock) {
  int flags = fcntl(sock, F_GETFL, 0);
  if (flags == -1) {
    printf("setNonBlocking: fcntl() err : '%s'\n", strerror(errno));
    return -1;
  }

  flags |= O_NONBLOCK;

  int result = fcntl(sock, F_SETFL, flags);
  if (result == -1) {
    printf("setNonBlocking: fcntl() err : '%s'\n", strerror(errno));
    return -1;
  }

  return result;
}

void acceptCallback(struct eventData *fdData,
                    struct eventState *state) {
  socklen_t addrlen = sizeof(struct sockaddr_in);
  struct sockaddr_in *clientAddr = malloc(sizeof(struct sockaddr_in));
  if (!clientAddr) {
    printf("acceptCallback: malloc() err : '%s'\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  int currentFileDescr =
      accept(fdData->currentFileDescr, (struct sockaddr *)clientAddr,
             &addrlen);
  int resSetNonBlock = setNonBlocking(currentFileDescr);
  if (resSetNonBlock == -1) {
    printf("errors in nonblocking\n");
    exit(EXIT_FAILURE);
  }
  if (currentFileDescr == -1) {
    printf("acceptCallback: accept() err : '%s'\n", strerror(errno));
  } else {
    fdData = eventSet(currentFileDescr, handshakeCallback, state);
    fdData->addr = clientAddr;
    printf("Connection from %s\n", convertAddrToStr(fdData->addr));
  }
}
