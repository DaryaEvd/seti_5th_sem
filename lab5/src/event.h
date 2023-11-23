#ifndef EVENT__
#define EVENT__
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

#define MAX_EVENTS 64

struct eventState {
  int epollFileDescr;
  int numberOfReadyFileDescrs;
};

struct eventData {
  int currentFileDescr;
  void (*callback)(struct eventData *fdData,
                   struct eventState *state);
  struct eventData *eventTo;
  struct sockaddr_in *addr;
};

typedef void (*callback)(struct eventData *, struct eventState *);

/* invoke epoll and sore file descrs to eventState structure*/
void initEvent(struct eventState *state);

/* waits for events, calls callback for each fd*/
void startEvent(struct eventState *state);

/* creates new eventData, sets up epoll event for fd*/
struct eventData *eventSet(int currentFileDescr, callback callback,
                           struct eventState *state);

/* resets number of fd */
void restartEvent(struct eventState *state);

/* removes the epoll event for a specific fd */
void clearEvent(struct eventData *fdData, struct eventState *state);

/* closes fd and free mem*/
void closeAndFree(struct eventData *fdData);

#endif // EVENT__
