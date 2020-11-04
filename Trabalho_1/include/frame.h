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


unsigned char buildInformationFrame(struct InformationFrame);