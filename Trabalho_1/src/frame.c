#include "frame.h"

int writeFrameWithFlags(int fd, char frame[], int frameLength)
{   
    /*char* a = frame;
    for (int i = 0; i < frameLength; i++) {
        printf("%X ", (uint8_t)*a);
        a++;
    }
    printf("\n");*/

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
        printf("Byte %i: %X \n", i, uns);
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

char* buildFrame(char* packet, int* packetLength, int index)
{
    if ((packet == NULL) || (packetLength <= 0))
    {
        printError("wth are you doing men... \n");
        exit(-1);
    }
    char* frame = malloc(sizeof(char) * (*packetLength + 4));
    char* iterator = frame;

    // insert address
    *iterator = FRAME_ADDRESS_FIELD_TYPE1;

    // insert control
    iterator++;
    *iterator = index == 0 ? FRAME_CONTROL_FIELD_SEND0 : FRAME_CONTROL_FIELD_SEND1;

    // insert BCC1
    iterator++;
    *iterator = (*(iterator - 1)) ^ (*(iterator - 2));

    // insert packet content and calculate BCC2, its easier when inserting :)
    iterator++;
    char bcc2 = 0;
    for (int i = 0; i < *packetLength; i++) {
        *iterator = *packet;
        bcc2 ^= *packet;
        iterator++;
        packet++;
    }
    *iterator = bcc2;
    *packetLength += 4;
    frame = stuffing(frame, packetLength);
    return frame;
}

char* unBuildFrame(char* frame, int* frameLength, int index)
{   
    if ((frame == NULL) || (*frameLength <= 0))
    {
        printError("wth are you doing men... \n");
        exit(-1);
    }
    *frameLength -=2;
    frame = destuffing(frame + 1, frameLength);

    //u_int8_t addressField = frame[1];
    u_int8_t controlField = frame[2]; 

    if (controlField == FRAME_CONTROL_FIELD_SEND0) 
        index = 0;
    else if (controlField == FRAME_CONTROL_FIELD_SEND1)
        index = 1;
    
    u_int8_t bcc1 = frame[2]; 

    // check if bcc is correct
    char correctBcc1 = frame[0] ^ frame[1];
    if (correctBcc1 != bcc1) {
        printError("Incorrect BCC1! \n");
        *frameLength = -1;
        return frame;
    }

    char correctBcc2 = 0;
    for (int i = 0; i < *frameLength - 4; i++) {
        correctBcc2 ^= frame[i + 3];
    }
    
    char bcc2 = frame[*frameLength - 1];
    
    if (correctBcc2 != bcc2) {
        printError("Error in bcc2! \n");
        *frameLength = -1;
        return frame;
    }

    *frameLength -= 4;

    return frame + 3;
}
