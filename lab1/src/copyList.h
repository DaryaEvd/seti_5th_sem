#ifndef COPYLIST_H
#define COPYLIST_H

#include <time.h>

typedef struct Copy {
  char *ip;
  time_t currentTime;
} Copy;

typedef struct CopyList {
  Copy value;
  struct CopyList *next;
} CopyList;

CopyList *refreshMembersList(CopyList *copyList, Copy copy);

void printMembers(CopyList *copyList);
void freeList(CopyList *copyList);

#endif // COPYLIST_H
