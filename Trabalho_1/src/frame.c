#include "frame.h"

int checkIfIsFrame(char buffer[], const unsigned char* targetFrame, int verbose)
{
    char* ptr = buffer;
    int isEqual = 1;

    for (int i=0; i<5; i++) {
        u_int8_t byte = *ptr;
        if (!(byte == targetFrame[i])) {
            if (verbose) {
                printf("Frame Byte %i is not the same! \n", i);
                printf("received byte %x , ", buffer[i]);
                printf("wanted byte %x \n", targetFrame[i]);
            }            
            isEqual = 0;
        }
        ptr++;
    }

    return isEqual;
}

int printFrame(char frame[], int frameSize)
{
    char* ptr = frame;
    for (int i = 0; i < frameSize; i++) {
        u_int8_t uns = *ptr;
        printf("Byte %i: %X \n", i, uns);
        ptr++;
    }

    return 0;
}

int checkFrameIndex(char responseBuffer[], int sentFrameIndex)
{
    // sent index frame must be opposite of the index of the received response
    if (sentFrameIndex == 0) {
        if (checkIfIsFrame(responseBuffer, FRAME_RR1, 0)) {
            printSuccess("Received RR! \n"); 
            return 1;
        }
        else if (checkIfIsFrame(responseBuffer, FRAME_REJ1, 0)) {
            printError("Received REJ! \n");
            return 1;
        }
        else {
            printError("Out of order! \n");
            return 0;
        }  
    }
    else if (sentFrameIndex == 1) {
        if (checkIfIsFrame(responseBuffer, FRAME_RR0, 0)) {
            printSuccess("Received RR! \n"); 
            return 1;
        }
        else if (checkIfIsFrame(responseBuffer, FRAME_REJ0, 0)) {
            printError("Received REJ! \n");
            return 1;
        }
        else {
            printError("Out of order! \n");
            return 0;
        }
    } 

    return 0;
}


