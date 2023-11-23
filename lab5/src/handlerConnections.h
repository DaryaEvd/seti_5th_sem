#ifndef STUFF__
#define STUFF__

#include <arpa/inet.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>

#define BUFFER_SIZE 10000

struct eventState *state;
struct eventData *fdData;

/*
Invokes when a relay between two sockets needs to be stopped.
It clears the epoll events for both sockets, closes the fd,
and restarts the epoll instance
*/
void stopCallback(struct eventData *fdData, struct eventState *state);

#endif // STUFF__
