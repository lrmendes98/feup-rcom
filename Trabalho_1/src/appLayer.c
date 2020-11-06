#include "appLayer.h"

int appLayerWrite(int fd)
{
    char packet[] = "vou apanhalos todos";

    // bufferSize = getFileBinary("testFiles/pinguim.gif", &buffer);
    llwrite(fd, packet, sizeof(packet));
    // free(buffer);
    // buffer = NULL;

    
    return 0;
}

int appLayerRead(int fd)
{
    char buffer[10000];
    // Read bytes
    llread(fd, buffer);

    //exportFile("receivedFiles/pinguim.gif", &buffer);


    return 0;
}
