#ifndef HANDLER_CONNECTION__
#define HANDLER_CONNECTION__

#include <arpa/inet.h>

#define BUFFER_SIZE 10000

struct eventState *state;
struct eventData *fdData;

void stopCallback(struct eventData *fdData, struct eventState *state);

#endif // HANDLER_CONNECTION__
