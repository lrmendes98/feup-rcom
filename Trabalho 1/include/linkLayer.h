#pragma once

#include <stdio.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <unistd.h>
#include "macros.h"


int llopen(char* porta, int mode);

int llclose(int fd);

int llread(int fd, char* buffer);

int llwrite(int fd, char* buffer, int length);

int llopenTransmitter(int fd);

int llopenReceiver(int fd);
