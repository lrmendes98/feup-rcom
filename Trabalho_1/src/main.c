#include "macros.h"
#include "variables.h"
#include "appLayer.h"

/* Global variables */
extern char* fileName;
extern int packetSize;
extern int timeoutSeconds;

int main(int argc, char *argv[])
{
    int operation = -1;

    // Handle program arguments
    if (strcmp("./read", argv[0]) == 0) {
        if (argc != 3) {
            printf("Usage: ./read <port> <packetSize>\n");
            exit(-1);
        }
        else {
            packetSize = atoi(argv[2]);
            operation = RECEIVER;
        }
    }
    if (strcmp("./write", argv[0]) == 0) {
        if (argc != 5) {
            printf("Usage: ./write <port> <fileName> <packetSize> <timeoutSeconds> <maxTries> <BAUDRATE> \n");
            exit(-1);
        }
        else {
            fileName = argv[2];
            packetSize = atoi(argv[3]);
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