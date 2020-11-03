#include "appLayer.h"

int appLayerWrite(int fd)
{
    char *buffer;
    long bufferSize = 0;
    bufferSize = getFileBinary("testFiles/pinguim.gif", &buffer);

    llwrite(fd, buffer, bufferSize);

    free(buffer);
    buffer = NULL;
    
    return 0;
}

int appLayerRead(int fd)
{
    char* buffer;
    // Read bytes
    llread(fd, buffer);

    exportFile("receivedFiles/pinguim.gif", &buffer);


    return 0;
}

