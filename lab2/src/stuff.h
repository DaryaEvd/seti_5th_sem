#ifndef STUFF
#define STUFF

#include <stdio.h>

int isExistingFile(const char *path);
int isValidFileNameLength(const char *fileName);
int isValidSizeFile(long long size);
char *extractLastToken(const char *inputPathToFile);
long long countSizeFile(FILE *file);
int createDir(const char *path);

#endif // STUFF
