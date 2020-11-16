#include "macros.h"
#include "appLayer.h"

int main(int argc, char *argv[])
{
    int operation = -1;

    // Handle program arguments
    if (argc == 1) {
        printf("Usage: %s <port> \n", argv[0]);
        return -1;
    }
    else if (argc > 2) {
        printf("Usage: %s <port> \n", argv[0]);
        return -1;
    }
    else {
        if (strcmp("./read", argv[0]) == 0) 
		    operation = RECEIVER;
        if (strcmp("./write", argv[0]) == 0)
            operation = TRANSMITTER;
    }
    
    // Open port
    int fd = llopen(argv[1], operation);
    
    if (fd < 1) 
        exit(-1);

    srand(time(NULL));
    
    if (operation == TRANSMITTER)
        appLayerWrite(fd);
    else if (operation == RECEIVER)
        appLayerRead(fd);  

    llclose(fd);  

    return 0;
}