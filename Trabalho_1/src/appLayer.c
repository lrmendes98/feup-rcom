#include "appLayer.h"

int packetSize = 8;


int appLayerWrite(int fd)
{
    u_int8_t byte1 = 0xAA;
    u_int8_t byte2 = 0xBB;
    u_int8_t byte3 = 0xBB;
    u_int8_t byte4 = 0xBB;
    u_int8_t byte5 = 0xBB;
    u_int8_t byte6 = 0xBB;
    u_int8_t byte7 = 0xBB;
    u_int8_t byte8 = 0xBB;

    char packet[8];
    packet[0] = byte1;
    packet[1] = byte2;
    packet[2] = byte3;
    packet[3] = byte4;
    packet[4] = byte5;
    packet[5] = byte6;
    packet[6] = byte7;
    packet[7] = byte8;

    llwrite(fd, packet, packetSize);
    
    return 0;
}

int appLayerRead(int fd)
{
    char buffer[packetSize];

    // Read bytes
    llread(fd, buffer);

    char * a = buffer;
    for (int i = 0; i < packetSize; i++) {
        u_int8_t uns = *a;
        printf("%X ", uns);
        a++;
    }
    printf("\n");

    //exportFile("receivedFiles/pinguim.gif", &buffer);


    return 0;
}
