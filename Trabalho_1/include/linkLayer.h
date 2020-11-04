#pragma once

#include <stdio.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <unistd.h>
#include <stdbool.h>
#include <termios.h>
#include "macros.h"
#include "auxiliar.h"
#include "frame.h"

/* Global Variables */
extern struct termios oldtio;


int setOldPortAttributes(int fd);

int getAndSaveOldPortAttributes(int fd);

int llopenTransmitter(int fd);

int llopenReceiver(int fd);

int llopen(char* porta, int mode);

/* Closes port
** @param fd: File descriptor
** @return: if successful, 1, if not, -1
*/
int llclose(int fd);

int llread(int fd, char* buffer);

/* Sends array buffer 
** @param fd: File descriptor
** @param buffer: pointer of buffer to send
** @param length: length of buffer
** @return: if successful, number of characters written, if not, -1
*/
int llwrite(int fd, char* buffer, int length);
