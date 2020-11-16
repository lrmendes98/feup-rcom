#include "frame.h"

int writeFrameWithFlags(int fd, char frame[], int frameLength)
{
    // char currentBcc1;
    // char currentBcc2;
    // char currentIndex;

    // if (ENABLE_CURRUPT_FRAME_TESTS) {
    //     currentBcc1 = frame[2];
    //     currentBcc2 = frame[frameLength - 2];
    //     currentIndex = frame[1];
    //     int res = rand() % 20;
    //     if (res == 0) {
    //         frame[2] = 0;
    //     }
    //     if (res == 1) {
    //         frame[frameLength - 2] = 0;
    //     }
    //     if (res == 2) {
    //         if (frame[1] == FRAME_CONTROL_FIELD_SEND0) {
    //             frame[1] = FRAME_CONTROL_FIELD_SEND1;
    //         }
    //         else {
    //             frame[1] = FRAME_CONTROL_FIELD_SEND0;
    //         }
    //     }
    // }

    char flag = FRAME_FLAG;
    int writtenSize = write(fd, &flag, 1);

    //send frame
    writtenSize += write(fd, frame, frameLength);
    writtenSize += write(fd, &flag, 1);
    
    //printf("Bytes written = %d\n", writtenSize);

    // if (ENABLE_CURRUPT_FRAME_TESTS) {
    //     frame[frameLength - 2] = currentBcc2;
    //     frame[2] = currentBcc1;
    //     frame[1] = currentIndex;
    // }
    
    return writtenSize;
}

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
        printf("%X ", uns);
        ptr++;
    }
    printf("\n");

    return 0;
}

int getFrameIndex(char frame[]) 
{
    // if is supervision frame
    if (checkIfIsFrame(frame, FRAME_RR0, 0) || 
            checkIfIsFrame(frame, FRAME_REJ0, 0)) {
        return 0;
    }
    if (checkIfIsFrame(frame, FRAME_RR1, 0) || 
            checkIfIsFrame(frame, FRAME_REJ1, 0)) {
        return 1;
    }

    return -1;
}

int buildFrame(char* packet, int* packetLength, int index, char* frame)
{
    if ((packet == NULL) || (*packetLength <= 0) || (frame == NULL) || 
        (index & ~1))
    {
        printError("wth are you doing men... \n");
        exit(-1);
    }

    // insert address
    frame[0] = FRAME_ADDRESS_FIELD_TYPE1;

    // insert control
    frame[1] = index == 0 ? FRAME_CONTROL_FIELD_SEND0 : FRAME_CONTROL_FIELD_SEND1;

    // insert BCC1
    frame[2] = frame[0] ^ frame[1];

    (*packetLength) += 3;

    stuffing(packet, packetLength, frame);

    //printf("bcc2: %X\n", bcc2);

    return 1;
}

int unBuildFrame(char* frame, int frameLength, char* outputPacket, char bcc2)
{
    if ((frame == NULL) || (frameLength <= 0) || (outputPacket == NULL)) {
        printError("wth are you doing men... \n");
        exit(-1);
    }

    //u_int8_t addressField = frame[0];
    
    u_int8_t bcc1 = frame[2]; 

    // check if bcc is correct
    char correctBcc1 = frame[0] ^ frame[1];

    if (correctBcc1 != bcc1) {
        printError("Incorrect BCC1! \n");
        return -1;
    }
    
    char correctBcc2 = frame[frameLength - 1];

    //printf("%X\n", (uint8_t)bcc2);
    
    if (correctBcc2 != bcc2) {
        printError("Incorrect bcc2! \n");
        //printf("%X\n", (uint8_t)correctBcc2);
        return -1;
    }    

    return 0;
}
