#ifndef HANDLER_TCP__
#define HANDLER_TCP__

#include "common.h"
#include "event.h"
#include "handlerConnections.h"

/*
Invokes when a TCP socket is ready.
It receives data from the socket, forwards it to another socket
*/
void runTcpCallback(struct eventData *fdData,
                    struct eventState *state);
void requestCallback(struct eventData *fdData,
                     struct eventState *state);

void handshakeCallback(struct eventData *fdData,
                       struct eventState *state);
void acceptCallback(struct eventData *fdData,
                    struct eventState *state);

#endif // HANDLER_TCP__
