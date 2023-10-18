#include "stuff.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int isExistingFile(const char *path) {
  if (access(path, F_OK) != 0) {
    printf("Such file by this path '%s' doesn't exists. Try to enter "
           "another file!\n",
           path);
    return 0;
  }
  return 1;
}

int isValidFileNameLength(const char *fileName) {
  if (strlen(fileName) > sizeof(char) * 4096) {
    printf("Your filename is too long. Rename it or give another "
           "one\n");
    return 0;
  }
  return 1;
}

char *extractLastToken(const char *inputPathToFile) {
  int amountSymbolsInLastToken = 0;

  for (int i = strlen(inputPathToFile) - 1; i > 0; i--) {
    if (inputPathToFile[i] == '/') {
      break;
    }
    amountSymbolsInLastToken++;
  }

  char *lastToken =
      calloc(amountSymbolsInLastToken + 1, sizeof(char));

  for (int i = 0; i < amountSymbolsInLastToken; i++) {
    lastToken[i] = inputPathToFile[i + (strlen(inputPathToFile) -
                                        amountSymbolsInLastToken)];
  }

  return lastToken;
}

long countSizeFile(FILE *file) {
  fseek(file, 0L, SEEK_END);
  long sizeFile = ftell(file);
  printf("size file: '%ld' bytes\n", sizeFile);
  rewind(file);
  return sizeFile;
}
