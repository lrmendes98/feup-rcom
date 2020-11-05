#include "frame.h"

int checkIfIsFrame(char* buffer, const unsigned char* targetFrame, int verbose)
{
    int isEqual = 1;
    for (int i=0; i<5; i++) {
        if (!(buffer[i] == targetFrame[i])) {
            if (verbose) {
                printf("Frame Byte %i is not the same! \n", i);
                printf("received byte %x , ", buffer[i]);
                printf("wanted byte %x \n", targetFrame[i]);
            }            
            isEqual = 0;
        }
    }

    return isEqual;
}
