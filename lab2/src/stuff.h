#ifndef STUFF
#define STUFF 

#include <stdio.h>

int isExistingFile(const char *path);
int isValidFileNameLength(const char *fileName) ;
char *extractLastToken(const char *inputPathToFile);
long countSizeFile(FILE *file);
int createDir(const char *path);

#endif // STUFF
