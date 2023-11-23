#include "commons.h"
#include "event.h"
#include "handlerTCP.h"
#include "handlerConnections.h"
#include "utils.h"
#include <errno.h>
 
int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return 0;
  }

  in_addr_t addr = htonl(INADDR_ANY);
  in_port_t port = htons(atoi(argv[1]));

  printf("welcome to proxy with no auth required\n");

  int currentFileDescr = startListen(addr, port);
  if (!currentFileDescr) {
    printf("startListen: socket() err : '%s'\n", strerror(errno));
    return 0;
  }
  printf("Listening on %s:%hu\n", convertToAddr(&addr), ntohs(port));

  struct eventState state;
  initEvent(&state);
  struct eventData *fdData =
      eventSet(currentFileDescr, acceptCallback, &state);

  startEvent(&state);
}
