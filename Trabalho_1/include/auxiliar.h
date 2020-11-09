#pragma once
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <stdio.h>
#include <time.h>


char* stringAdd(const char *s1, const char *s2);

long getFileBinary(char *path, char** buffer);

char* generateRdmBytestream(size_t num_bytes);

char* printError(char* msg);

char* printWarning(char* msg);

char* printSuccess(char* msg);

/** Creates a file with received content (binary)
 * @param path: New file path
 * @param content: New file binary
*/
int exportFile(char* path, char** content); 

