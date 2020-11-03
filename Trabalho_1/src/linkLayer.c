#include "linkLayer.h"


int llopen(char* porta, int mode)
{
    int fd = open(porta, O_RDWR);

    if (fd < 0) {
        printf("Error opening port! %s", porta);
        return -1;
    }

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

int llread(int fd, char* buffer)
{   
    int i, res = 0;

    while(1) {
        res = read(fd,buffer,255);
        // buffer[res]='\0';             /* set end of string, so we can printf */
        // for (i = 0; i < res; i++) printf("%c", buffer[i]);
        // printf("\n");
        // if (buffer[0] == 'z');
        printf("buffer: %s\n", buffer);
    }

    return 0;
}

int llwrite(int fd, char* buffer, int length)
{
    int bytes = 0;
      
    bytes = write(fd,buffer,255);
    printf("Bytes written = %d\n", bytes);
    
    return 0;
}

int llopenTransmitter(int fd)
{
    
    return 0;
}

int llopenReceiver(int fd)
{
    char* buffer;

    llread(fd, buffer);

    return 0;
}