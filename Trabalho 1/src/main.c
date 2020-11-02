#include "macros.h"
#include "linkLayer.h"

int main(int argc, char *argv[])
{
    int operation = -1;

    // Handle program arguments
    if (argc == 1) {
        printf("Usage: ./rcom <read|write> <port> \n");
        return -1;
    }
    else if(argc > 3 || ((strcmp("read", argv[1]) != 0) && (strcmp("write", argv[1]) != 0))) {
        printf("Usage: ./rcom <read|write> <port> \n");
        return -1;
    }    
    else {
        if (strcmp("read", argv[1]) == 0)
		    operation = RECEIVER;
        if (strcmp("write", argv[1]) == 0)
            operation = TRANSMITTER;
    }

    printf("Port: %s\n", argv[2]);
    printf("operation: %i", operation);
    printf("\n");

    int fd = llopen(argv[2], operation);

    // if (operation == TRANSMITTER)
    //     appLayerWrite(fd);
    // else if (operation == RECEIVER)
    //     appLayerRead(fd);
    

    return 0;
}