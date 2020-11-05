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
#include <signal.h>

#include "macros.h"
#include "auxiliar.h"
#include "frame.h"

/* Global Variables */
extern struct termios oldtio;
extern unsigned int counterTries;



/* atende alarme */
void atende();

/**
 * Sets old port attributes
 * @param fd: Port field descriptor
 * @returns: 1 if succeeded, if fails, exits program
 */
int setOldPortAttributes(int fd);

/** 
 * Saves old port attributes and sets new ones
 * @param fd: Port file descriptor
 * @returns: 1 if succeeded, if fails, exits program
 */
int portAttributesHandler(int fd);

/** 
 * Opens and validates link on the transmitter side
 * @param fd: File descriptor
 * @return: 1 if succeeded, -1 if failed
*/
int llopenTransmitter(int fd);

/**
 * Opens and validates link on the receiver side
 * @param fd: File descriptor
 * @return: 1 if succedeed, -1 if not
*/
int llopenReceiver(int fd);

int llopen(char* porta, int mode);

/** 
 * Closes port
 * @param fd: File descriptor
 * @return: if successful, 1, if not, -1
*/
int llclose(int fd);

int llread(int fd, char* buffer);

/** 
 * Sends array buffer 
 * @param fd: File descriptor
 * @param buffer: pointer of buffer to send
 * @param length: length of buffer
 * @return: if successful, number of characters written, if not, -1
*/
int llwrite(int fd, char* buffer, int length);
