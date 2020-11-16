#include "macros.h"
#include "appLayer.h"

/* Global input variables */
extern char* fileName;
extern int packetSize;
extern int timeoutSeconds;
extern int maxTries;
extern int baudrate;

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
        if (argc != 7) {
            printf("Usage: ./write <port> <fileName> <packetSize> <timeoutSeconds> <maxTries> <baudrate> \n");
            exit(-1);
        }
        else {
            fileName = argv[2];
            packetSize = atoi(argv[3]);
            timeoutSeconds = atoi(argv[4]);
            maxTries = atoi(argv[5]);
            baudrate = atoi(argv[6]);

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