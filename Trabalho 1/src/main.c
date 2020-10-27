#include <stdio.h>
#include <string.h>
#include "aux.h"

int main(int argc, char *argv[])
{
    char operation[100] = "";
    char port[100] = "";


    // Handle program arguments
    if (argc > 3 || ((strcmp("read", argv[1]) != 0) && (strcmp("write", argv[1]) != 0))) {
        printf("./rcom <read|write> <port> \n");
        return 1;
    }    
    else {
        if (strcmp("read", argv[1]) == 0)
		    strcpy(operation, "TRANSMITTER");
        if (strcmp("write", argv[1]) == 0)
            strcpy(operation, "RECEIVER");

        strcpy(port, argv[2]);
    }

    printf("Port: %s", port);
    printf("operation: %s", operation);
    printf("\n");

    return 0;
}