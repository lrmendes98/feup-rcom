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
 * Receives a frame
 * Reads the port until a start flag is detected.
 * If so, it allocates each read byte into the buffer until a new 
 * frame flag is detected, meaning it's the end of that frame
 * TODO: if doesn't receive end flag, do something
 * @param fd: Port file descriptor
 * @param buffer: Buffer with the received frame
 * @return: Number of bytes of received frame, -1 if fails
 */
int receiveFrame(int fd, char* buffer);

/** 
 * Answers SIGALRM interrupts, increments counterTries
 */
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

/**
 * Opens and establishes serial port connections
 * @param porta: String of port path
 * @param mode: RECEIVER or TRANSMITTER
 * @return: If succeeded, returns port file descriptor. If fails, returns negative
 */
int llopen(char* porta, int mode);

/** 
 * Closes port
 * @param fd: File descriptor
 * @return: if successful, 1, if not, -1
*/
int llclose(int fd);

/**
 * Reads port
 * @param fd: Port file descriptor
 * @param buffer: Array of the packet
 * @return: If succeeded, returns number of received chars. If fails, returns negative
 */
int llread(int fd, char* buffer);

/** 
 * Sends array buffer 
 * @param fd: File descriptor
 * @param buffer: pointer of buffer to send
 * @param length: length of buffer
 * @return: if successful, number of characters written, if not, -1
*/
int llwrite(int fd, char* buffer, int length);
