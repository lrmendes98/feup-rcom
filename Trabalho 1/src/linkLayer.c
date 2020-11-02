#include "linkLayer.h"


int llopen(char* porta, int mode)
{
    int fd = open(porta, O_RDWR);

    if (mode == RECEIVER)
        llopenReceiver(fd);
    else if (mode == TRANSMITTER)
        llopenTransmitter(fd);

    return fd;
}

int llclose(int fd)
{

    return 0;
}

int llread(int fd, char *buffer)
{

    return 0;
}

int llwrite(int fd, char *buffer, int length)
{

    return 0;
}

int llopenTransmitter(int fd)
{

    return 0;
}

int llopenReceiver(int fd)
{

    return 0;
}