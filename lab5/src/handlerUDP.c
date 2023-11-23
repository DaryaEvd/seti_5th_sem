#include "commons.h"
 #include "event.h"
#include "handlerConnections.h"
#include "utils.h"
#include <errno.h>
#include <fcntl.h>
 
void stopUdpCallback(struct eventData *fdData) {
  printf("UDP relay stopped\n");
  stopCallback(fdData, state);
}

void runUdpCallback(struct eventData *fdData,
                    struct eventState *state) {
  printf("client: %s\n", convertAddrToStr(fdData->addr));
  // recv data from client
  uint8_t recvDataFromClient[BUFFER_SIZE] = {0};

  // source addr from which udp packet receivved
  struct sockaddr_in peerAddr;
  socklen_t addrlen = sizeof(struct sockaddr_in);
  int bufferLength = recvfrom(fdData->currentFileDescr,
                              recvDataFromClient, BUFFER_SIZE, 0,
                              (struct sockaddr *)&peerAddr, &addrlen);
  if (bufferLength <= 0) {
    printf("runUdpCallback: recvfrom() err : '%s'\n",
           strerror(errno));
    printf("client: %s\n", convertAddrToStr(fdData->addr));
    stopUdpCallback(fdData);
    return;
  }
  printf("peer: %s\n", convertAddrToStr(&peerAddr));

  // check either we dropp recieved packet or not
  if (peerAddr.sin_addr.s_addr == fdData->addr->sin_addr.s_addr &&
      peerAddr.sin_port == fdData->addr->sin_port) {

    struct datagramStruct *dgram =
        (struct datagramStruct *)recvDataFromClient;
    if (dgram->atyp == IPV6) {
      printf("Dropped\n");
      printf("client: %s\n", convertAddrToStr(fdData->addr));
      printf("peer: %s\n", convertAddrToStr(&peerAddr));
      return;
    }

    // extract dest addr and port from recv dgram
    struct sockaddr_in dstAddrDgram;
    memset(&dstAddrDgram, 0, sizeof(struct sockaddr_in));
    dstAddrDgram.sin_family = AF_INET;
    dstAddrDgram.sin_port = getDstPort(&dgram->dst, dgram->atyp);
    dstAddrDgram.sin_addr.s_addr =
        getDstAddr(&dgram->dst, dgram->atyp);
    printf("client -> %s\n", convertAddrToStr(&dstAddrDgram));

    int payloadLen;
    uint8_t *payload = getPayload(dgram, bufferLength, &payloadLen);
    printf("payload: \n");

    if (sendto(fdData->currentFileDescr, payload, payloadLen, 0,
               (struct sockaddr *)&dstAddrDgram,
               sizeof(struct sockaddr_in)) == -1) {
      printf("runUdpCallback: sendto() err : '%s'\n",
             strerror(errno));
      printf("client: %s\n", convertAddrToStr(fdData->addr));
      printf("peer: %s\n", convertAddrToStr(&peerAddr));
      printf("client -> %s\n", convertAddrToStr(&dstAddrDgram));
      printf("payload: \n");
    } else {
      printf("Payload sent successfully\n");
    }
  } else {
    // handle situation when the packet is from the peer to the client
    printf("client <- %s\n", convertAddrToStr(&peerAddr));
    uint8_t replayClient[BUFFER_SIZE + DGRAM_IPV4_SIZE] = {0};
    struct datagramStruct *dgram =
        (struct datagramStruct *)replayClient;

    // prepare new datagramStruct with the client's addr as the dest
    dgram->atyp = IPV4;
    dgram->dst.ipv4.addr = peerAddr.sin_addr.s_addr;
    dgram->dst.ipv4.port = peerAddr.sin_port;
    memcpy((uint8_t *)&dgram->dst.ipv4.port + 2, recvDataFromClient,
           bufferLength);

    // send the new datagramStruct to the client
    if (sendto(fdData->currentFileDescr, replayClient,
               bufferLength + DGRAM_IPV4_SIZE, 0,
               (struct sockaddr *)fdData->addr,
               sizeof(struct sockaddr_in)) == -1) {

      printf("runUdpCallback: sendto() err : '%s'\n",
             strerror(errno));
      printf("client: %s\n", convertAddrToStr(fdData->addr));
      printf("peer: %s\n", convertAddrToStr(&peerAddr));
    } else {
      printf("Packet sent successfully\n");
    }
  }
}
