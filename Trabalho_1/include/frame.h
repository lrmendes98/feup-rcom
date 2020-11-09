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
#include "stuffing.h"


static const unsigned char FRAME_SET[] = {FRAME_FLAG,
                                        FRAME_ADDRESS_FIELD_TYPE1,
                                        FRAME_CONTROL_FIELD_SET,
                                        (FRAME_ADDRESS_FIELD_TYPE1 ^ FRAME_CONTROL_FIELD_SET),
                                        FRAME_FLAG};

static const unsigned char FRAME_UA[] = {FRAME_FLAG,
                                        FRAME_ADDRESS_FIELD_TYPE1,
                                        FRAME_CONTROL_FIELD_UA,
                                        (FRAME_ADDRESS_FIELD_TYPE1 ^ FRAME_CONTROL_FIELD_UA),
                                        FRAME_FLAG};

static const unsigned char FRAME_REJ0[] = {FRAME_FLAG, 
                                        FRAME_ADDRESS_FIELD_TYPE1, 
                                        FRAME_CONTROL_FIELD_REJ0,
                                        (FRAME_ADDRESS_FIELD_TYPE1^FRAME_CONTROL_FIELD_REJ0),
                                        FRAME_FLAG};

static const unsigned char FRAME_REJ1[] = {FRAME_FLAG,
                                        FRAME_ADDRESS_FIELD_TYPE1,
                                        FRAME_CONTROL_FIELD_REJ1,
                                        (unsigned char)(FRAME_ADDRESS_FIELD_TYPE1^FRAME_CONTROL_FIELD_REJ1),
                                        FRAME_FLAG};

static const unsigned char FRAME_RR0[]  = {FRAME_FLAG,
                                        FRAME_ADDRESS_FIELD_TYPE1,
                                        FRAME_CONTROL_FIELD_RR0,
                                        (FRAME_ADDRESS_FIELD_TYPE1^FRAME_CONTROL_FIELD_RR0),
                                        FRAME_FLAG};

static const unsigned char FRAME_RR1[]  = {FRAME_FLAG,
                                        FRAME_ADDRESS_FIELD_TYPE1,
                                        FRAME_CONTROL_FIELD_RR1,
                                        (unsigned char)(FRAME_ADDRESS_FIELD_TYPE1^FRAME_CONTROL_FIELD_RR1),
                                        FRAME_FLAG};

static const unsigned char FRAME_DISC[] = {FRAME_FLAG,
                                        FRAME_ADDRESS_FIELD_TYPE1,
                                        FRAME_CONTROL_FIELD_DISC,
                                        (FRAME_ADDRESS_FIELD_TYPE1^FRAME_CONTROL_FIELD_DISC),
                                        FRAME_FLAG};


struct InformationFrame {
    u_int8_t flag;
    u_int8_t addressField;
    u_int8_t controlField;
    u_int8_t headerProtectionField;
    char* informationField;  
    u_int8_t bodyProtectionField;
};

int unBuildFrame(char* frame, int frameLength, int index, char* outputPacket);

int writeFrameWithFlags(int fd, char frame[], int frameLength);

/**
 * Builds an Information Frame with received packet and
 * returns a frame
 * @param packet: Received packet content
 * @param packetLength: Received packet length
 * @param frame: Output argument that contains the complete frame
 * @return: 1 if success, 0 if fails
 */ 
int buildFrame(char* packet, int packetLength, int index, char* frame);

/**
 * Returns the frame index
 * @param frame: Frame to be checked
 * @return: Frame index, -1 if fails
 */
int getFrameIndex(char frame[]);

/** 
 * Checks if buffer is a frame equal to targetFrame
 * @param buffer: char pointer of array of bytes to be checked
 * @param targetFrame: frame that buffer must be equal to
 * @param verbose: if 1(true), prints the bytes that are mismatched, 
 * if 0(false), no prints
 * @return: 1 if successful, 0 if not
*/
int checkIfIsFrame(char buffer[], const unsigned char* targetFrame, int verbose);

int printFrame(char frame[], int frameSize);
