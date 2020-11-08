#include "appLayer.h"

int packetSize = 4;


int appLayerWrite(int fd)
{
    u_int8_t byte1 = 0xAA;
    u_int8_t byte2 = 0xBB;
    u_int8_t byte3 = 0xCC;
    u_int8_t byte4 = 0xDD;
    
    char packet1[packetSize];
    packet1[0] = byte1;
    packet1[1] = byte2;
    packet1[2] = byte3;
    packet1[3] = byte4;

    char packet2[packetSize];
    packet2[0] = byte3;
    packet2[1] = byte2;
    packet2[2] = byte1;
    packet2[3] = byte4;

    char packet3[packetSize];
    packet3[0] = byte3;
    packet3[1] = byte2;
    packet3[2] = byte1;
    packet3[3] = byte4;

    char packet4[packetSize];
    packet4[0] = byte3;
    packet4[1] = byte2;
    packet4[2] = byte1;
    packet4[3] = byte1;

    llwrite(fd, packet1, packetSize);
    //llwrite(fd, packet2, packetSize);
    // llwrite(fd, packet3, packetSize);
    // llwrite(fd, packet4, packetSize);

    
    return 0;
}

int appLayerRead(int fd)
{
    char buffer[packetSize];

    // Read bytes
    llread(fd, buffer);


    char* a = buffer;
    for (int i = 0; i < packetSize; i++) {
        u_int8_t uns = *a;
        printf("%X ", uns);
        a++;
    }
    printf("\n");

    // llread(fd, buffer);

    // a = buffer;
    // for (int i = 0; i < packetSize; i++) {
    //     u_int8_t uns = *a;
    //     printf("%X ", uns);
    //     a++;
    // }
    // printf("\n");

    //llread(fd, buffer);

    // char * a = buffer;
    // for (int i = 0; i < packetSize; i++) {
    //     u_int8_t uns = *a;
    //     printf("%X ", uns);
    //     a++;
    // }
    printf("\n");

    //llread(fd, buffer);

    // char * a = buffer;
    // for (int i = 0; i < packetSize; i++) {
    //     u_int8_t uns = *a;
    //     printf("%X ", uns);
    //     a++;
    // }
    printf("\n");

    //exportFile("receivedFiles/pinguim.gif", &buffer);


    return 0;
}
