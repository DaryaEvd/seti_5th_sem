#ifndef STUFF
#define STUFF

#include <stdio.h>
#include <sys/types.h>

#define BUFFER_SIZE 30 

#define MSG_LENGTH 100
#define PATH_LENGTH 4096

#define BYTE 1
#define KILOBYTE 1024
#define MEGABYTE 1024 * 1024
#define GIGABYTE 1024 * 1024 * 1024

int isExistingFile(const char *path);
int isValidFileNameLength(const char *fileName);
int isValidSizeFile(long long size);
char *extractLastToken(const char *inputPathToFile);
__off64_t countSizeFile(char *fullPathToFileToSend);
int createDir(const char *path);

#endif // STUFF
