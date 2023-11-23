#ifndef HANDLER_UDP__
#define HANDLER_UPP__

void stopUdpCallback(struct eventData *fdData);

/*
Invokes when a UDP socket is ready.
It receives data from the socket, works with it and sends
it to another socket if necessary
*/
void runUdpCallback(struct eventData *fdData,
                    struct eventState *state);

#endif // HANDLER_UDP__
