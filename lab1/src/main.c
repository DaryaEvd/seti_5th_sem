#include "listener.h"
#include "sender.h"

#include <pthread.h>
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Bad input. Enter: [ ip group ] [ portNum number ] \n");
    return 0;
  }

  int stateOfPthread;

  pthread_t listenerThread;
  if ((stateOfPthread = pthread_create(
           &listenerThread, NULL, listenThreadWorks, argv)) != 0) {
    perror("error in listener thread creating");
    return 0;
  }

  pthread_t senderThread;
  if ((stateOfPthread = pthread_create(&senderThread, NULL,
                                       sendThreadWorks, argv)) != 0) {
    perror("error in sender thread creating");
    return 0;
  }

  if ((stateOfPthread = pthread_join(listenerThread, NULL)) != 0) {
    perror("error in listener joining");
    return 0;
  }

  if ((stateOfPthread = pthread_join(senderThread, NULL)) != 0) {
    perror("error in sender joining");
    return 0;
  }

  return 0;
}
