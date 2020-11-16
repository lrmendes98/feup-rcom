#include "macros.h"
#include "variables.h"
#include "appLayer.h"

/* Global variables */
char* fileName;

int main(int argc, char *argv[])
{
    int operation = -1;

    // Handle program arguments
    if (strcmp("./read", argv[0]) == 0) {
        if (argc != 2) {
            printf("Usage: ./read <port>\n");
            exit(-1);
        }
        else {
            operation = RECEIVER;
        }
    }
    if (strcmp("./write", argv[0]) == 0) {
        if (argc != 3) {
            printf("Usage: ./write <port> <fileName> <packetSize> <timeoutSeconds> <maxTries> <BAUDRATE> \n");
            exit(-1);
        }
        else {
            fileName = argv[2];
            operation = TRANSMITTER;
        }
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