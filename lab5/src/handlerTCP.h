#ifndef HANDLER_TCP__
#define HANDLER_TCP__

#include "common.h"
#include "event.h"
#include "handlerConnections.h"

/*
Implements a TCP handshake operation
*/
void handshakeCallback(struct eventData *fdData,
                       struct eventState *state);

/*
Invokes when a TCP socket is ready.
It receives data from the socket, forwards it to another socket
*/
void runTcpCallback(struct eventData *fdData,
                    struct eventState *state);

/*
Handles incoming SOCKS requests from clients.
It receives the client's request, validates it,and processes it to
establish a connection with the requested dest
*/
void requestCallback(struct eventData *fdData,
                     struct eventState *state);

/*
It accepts the request and sets the socket to non-blocking mode
*/
void acceptCallback(struct eventData *fdData,
                    struct eventState *state);

#endif // HANDLER_TCP__
