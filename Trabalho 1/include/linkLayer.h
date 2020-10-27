#pragma once
#include <string.h>
#include <stdlib.h>
#include "macros.h"

int llopen(int porta, int mode);

int llclose(int fd);

int llread(int fd, char * buffer);

int llwrite(int fd, char *buffer, int length);