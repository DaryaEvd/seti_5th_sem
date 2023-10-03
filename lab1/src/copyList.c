#include "copyList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



CopyList *refreshMembersList(CopyList *copyList, Copy copy) {
  if (copyList == NULL) {
    CopyList *node = malloc(sizeof(CopyList));
    node->value = copy;
    node->next = NULL;
    printMembers(node);
    return node;
  }

  const int delay = 5;

  int copyAmount = 0;

  CopyList *tmp = copyList;
  CopyList *prev;

  while (tmp != NULL) {
    if (strcmp(tmp->value.ip, copy.ip) == 0) {
      tmp->value.currentTime = time(NULL);
      copyAmount++;
    }

    if (difftime(time(NULL), tmp->value.currentTime) > delay) {
      prev->next = tmp->next;
      free(tmp);
      printMembers(copyList);
      return copyList;
    }

    prev = tmp;
    tmp = tmp->next;
  }

  if (copyAmount == 0) {
    CopyList *node = malloc(sizeof(CopyList));
    node->value = copy;
    node->next = NULL;
    prev->next = node;
    printMembers(copyList);
    return copyList;
  }

  return copyList;
}

void printMembers(CopyList *copyList) {
  printf("Current connections:\n");
  while (copyList != NULL) {
    printf("%s\n", copyList->value.ip);
    copyList = copyList->next;
  }
  printf("_______________________\n");
}

void free_list(CopyList *copyList) {
  CopyList *tmp;

  while (copyList != NULL) {
    tmp = copyList;
    copyList = copyList->next;
    free(tmp);
  }
}
