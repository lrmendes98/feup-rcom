#pragma once
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <stdio.h>
#include <time.h>


char* stringAdd(const char *s1, const char *s2);

char* getFileBinary(char* path);

char* generateRdmBytestream(size_t num_bytes);
