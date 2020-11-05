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

/** 
 * Checks if buffer is a frame equal to targetFrame
 * @param buffer: char pointer of array of bytes to be checked
 * @param targetFrame: frame that buffer must be equal to
 * @param verbose: if 1(true), prints the bytes that are mismatched, 
 *                 if 0(false), no prints
 * @return: 1 if successful, 0 if not
*/
int checkIfIsFrame(char* buffer, const unsigned char* targetFrame, int verbose);

/* atende alarme */
void atende();

int setOldPortAttributes(int fd);

int getAndSaveOldPortAttributes(int fd);

/** 
 * Opens and validates link on the transmitter side
 * @param fd: File descriptor
 * @return: 1 if succedeed, -1 if not
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
