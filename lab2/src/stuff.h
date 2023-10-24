#ifndef STUFF
#define STUFF

#include <stdio.h>

int isExistingFile(const char *path);
int isValidFileNameLength(const char *fileName);
int isValidEncoding(const char *filename);
char *extractLastToken(const char *inputPathToFile);
unsigned long countSizeFile(FILE *file);
int createDir(const char *path);

#endif // STUFF
