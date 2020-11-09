#include "frame.h"

int writeFrameWithFlags(int fd, char frame[], int frameLength)
{
    char flag = FRAME_FLAG;
    int writtenSize = write(fd, &flag, 1);

    //send frame
    writtenSize += write(fd, frame, frameLength);
    writtenSize += write(fd, &flag, 1);
    
    printf("Bytes written = %d\n", writtenSize);
    
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
        //printf("Byte %i: %X \n", i, uns);
        printf("%X ", uns);
        ptr++;
    }

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

    // TODO: if is information frame...



    return -1;
}

int buildFrame(char* packet, int packetLength, int index, char* frame)
{
    if ((packet == NULL) || (packetLength <= 0) || (frame == NULL) || 
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
    printf("bcc1: %X\n", frame[2]);


    // insert packet content and calculate BCC2, its easier when inserting :)
    char bcc2 = 0;
    for (int i = 0; i < packetLength; i++) {
        frame[i + 3] = packet[i];
        bcc2 ^= packet[i];
    }

    printf("bcc2: %X\n", bcc2);

    // insert BCC2
    frame[packetLength + 3] = bcc2;

    return 1;
}

int unBuildFrame(char* frame, int frameLength, int receivedIndex, char* outputPacket)
{
    if ((frame == NULL) || (frameLength <= 0) || (outputPacket == NULL)) {
        printError("wth are you doing men... \n");
        exit(-1);
    }

    //u_int8_t addressField = frame[0];
    u_int8_t controlField = frame[1]; 

    if (controlField == FRAME_CONTROL_FIELD_SEND0) 
        receivedIndex = 0;
    else if (controlField == FRAME_CONTROL_FIELD_SEND1)
        receivedIndex = 1;
    
    u_int8_t bcc1 = frame[2]; 

    // check if bcc is correct
    char correctBcc1 = frame[0] ^ frame[1];

    if (correctBcc1 != bcc1) {
        printError("Incorrect BCC1! \n");
        return -1;
    }

    char correctBcc2 = 0;
    for (int i = 0; i < frameLength - 4; i++) {
        outputPacket[i] = frame[i + 3];
        correctBcc2 ^= outputPacket[i];
    }
    
    char bcc2 = frame[frameLength - 1];
    
    if (correctBcc2 != bcc2) {
        printError("Incorrect bcc2! \n");
        return -1;
    }    

    return 0;
}

