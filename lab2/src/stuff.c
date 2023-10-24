#include "stuff.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>

int isExistingFile(const char *pathToFile) {
  if (access(pathToFile, F_OK) != 0) {
    return 0;
  }
  return 1; // exists
}

int isValidFileNameLength(const char *fileName) {
  struct stat fileInfo;
  stat(fileName, &fileInfo);

  wchar_t wideFileName[4096];
  mbstowcs(wideFileName, fileName, 4096);

  int fileNameLength = wcslen(wideFileName);
  if(fileNameLength > 4096) {
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

unsigned long countSizeFile(FILE *file) {
  fseek(file, 0L, SEEK_END);
  long double sizeFile = ftell(file);
  // printf("size file: '%ld' kBytes\n", sizeFile / 1024);
  rewind(file);
  return sizeFile;
}

int createDir(const char *path) {
  struct stat st = {0};
  if (stat(path, &st) == -1) {
    printf("server: creating an 'uploads' folder\n");
    if (mkdir(path, S_IROTH | S_IWOTH | S_IXOTH | S_IRUSR | S_IWUSR |
                        S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP) ==
        -1) {
      printf("Error: %s\n", strerror(errno));
      return -1;
    }
  } else {
    printf("server: 'uploads' folder already exists\n");
  }
  return 0;
}
