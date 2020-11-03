#include "appLayer.h"

int appLayerWrite(int fd)
{
    // Write random bytes
    // char buf[255];
    // for (int i = 0; i < 254; i++) buf[i] = 'a' + i % 26;
    // buf[254] = '\n';

    char* buffer;

    //buffer = getFileBinary("testFiles/meme.png");

    buffer = generateRdmBytestream(254);

    llwrite(fd, buffer, sizeof(buffer));
    
    return 0;
}

int appLayerRead(int fd)
{
    char* buffer;

    // Read bytes
    llread(fd, buffer);


    
    return 0;
}

