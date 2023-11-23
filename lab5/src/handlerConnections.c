#include "handlerConnections.h"
#include "event.h"
#include "utils.h"

void stopCallback(struct eventData *fdData,
                  struct eventState *state) {
  printf("from %s", convertAddrToStr(fdData->addr));
  printf("\t to %s\n", convertAddrToStr(fdData->eventTo->addr));
  clearEvent(fdData->eventTo, state);
  closeAndFree(fdData->eventTo);
  clearEvent(fdData, state);
  closeAndFree(fdData);
  restartEvent(state);
}
