#include "event.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initEvent(struct eventState *state) {
  state->epollFileDescr = epoll_create(1);
  if (state->epollFileDescr == -1) {
    printf("initEvent: epoll_create() err : '%s'\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void startEvent(struct eventState *state) {
  struct epoll_event events[MAX_EVENTS];
  struct eventData *fdData;
  for (;;) {
    state->numberOfReadyFileDescrs =
        epoll_wait(state->epollFileDescr, events, MAX_EVENTS, 5000);
    if (state->numberOfReadyFileDescrs > 0) {
      for (int i = 0; i < state->numberOfReadyFileDescrs; i++) {
        fdData = events[i].data.ptr;
        fdData->callback(fdData, state);
      }
    } else if (state->numberOfReadyFileDescrs == 0) {
    } else {
      printf("startEvent: epoll_wait() err : '%s'\n",
             strerror(errno));
      exit(EXIT_FAILURE);
    }
  }
}

struct eventData *eventSet(int currentFileDescr, callback callback,
                           struct eventState *state) {
  struct eventData *fdData = malloc(sizeof(struct eventData));
  if (!fdData) {
    printf("eventSet: malloc() err : '%s'\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  fdData->currentFileDescr = currentFileDescr;
  fdData->callback = callback;
  fdData->eventTo = NULL;
  fdData->addr = NULL;

  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.ptr = fdData;

  // sets up epoll event for fd
  if (epoll_ctl(state->epollFileDescr, EPOLL_CTL_ADD,
                currentFileDescr, &ev) == -1) {
    printf("eventSet: epoll_ctl() err : '%s'\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  return fdData;
}

void restartEvent(struct eventState *state) {
  state->numberOfReadyFileDescrs = 0;
}

void clearEvent(struct eventData *fdData, struct eventState *state) {
  // removes the epoll event for a specific fd
  if (epoll_ctl(state->epollFileDescr, EPOLL_CTL_DEL,
                fdData->currentFileDescr, NULL) == -1) {
    printf("clearEvent: epoll_ctl() err : '%s'\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
}

void closeAndFree(struct eventData *fdData) {
  close(fdData->currentFileDescr);
  if (fdData->addr) {
    free(fdData->addr);
  }
  free(fdData);
}
