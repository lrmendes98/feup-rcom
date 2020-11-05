#include "linkLayer.h"


/* Global Variables */
struct termios oldtio;
unsigned int counterTries = 0;


void atende()
{
    printWarning("Timeout #");
    counterTries++;
    printf("%i \n", counterTries);
}

int setOldPortAttributes(int fd) 
{
    if (tcsetattr(fd,TCSANOW,&oldtio) == -1) {
      printError("tcsetattr has failed!");
      exit(-1);
    }
    return 1;
}

int portAttributesHandler(int fd)
{
    // save current port settings
    if (tcgetattr(fd, &oldtio) == -1) { 
      perror("tcgetattr has failed!");
      exit(-1);
    }

    struct termios newtio;

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    newtio.c_lflag = 0;

    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;

    tcflush(fd, TCIOFLUSH);

    if (tcsetattr(fd,TCSANOW,&newtio) == -1) {
      printError("tcsetattr has failed!");
      exit(-1);
    }

    return 1;
}

int llopenTransmitter(int fd)
{
    int currentCount, readSize = 0;
    char buffer[FRAME_SUPERVISION_SIZE];

    write(fd, FRAME_SET, FRAME_SUPERVISION_SIZE);

    alarm(TIMEOUT);    
    
    while(counterTries < MAXTRIES) {
        if (currentCount != counterTries) {
            currentCount = counterTries;
            alarm(TIMEOUT);
            write(fd, FRAME_SET, FRAME_SUPERVISION_SIZE);
        }
        
        readSize = read(fd, &buffer, FRAME_SUPERVISION_SIZE);
        
        if (readSize == FRAME_SUPERVISION_SIZE) 
            break;
    }

    if (counterTries == MAXTRIES) {
        printError("Exceeded MAXTRIES!\n");
        return -1;
    }

    if (checkIfIsFrame(buffer, FRAME_UA, 0)) {
        printSuccess("Received UA! \n");
        return 1;
    }
    else if (checkIfIsFrame(buffer, FRAME_REJ0, 0)) {
        printError("Received frame is REJ0! \n");
        return -1;
    }
    else {
        printError("Didn't recognize frame! \n");
        return -1;
    }
      
    return 1;
}

int llopenReceiver(int fd)
{
    // read frames
    char buffer[FRAME_SUPERVISION_SIZE];
    
    int readSize = 0;

    while(readSize != FRAME_SUPERVISION_SIZE) {
        readSize = read(fd, &buffer, FRAME_SUPERVISION_SIZE);
    }
    
    if (checkIfIsFrame(buffer, FRAME_SET, 0)) {
        printSuccess("Received SET\n");
        // Send UA
        write(fd, FRAME_UA, FRAME_SUPERVISION_SIZE);    
        return 1;    
    }
    else {
        printError("Rejected Link!\n");
        write(fd, FRAME_REJ0, FRAME_SUPERVISION_SIZE);
        return -1;
    }

    return 1;
}

int llopen(char* porta, int mode)
{
    int fd = open(porta, O_RDWR | O_NOCTTY);

    if (fd < 0) {
        printf("Error opening port! %s", porta);
        return -1;
    }

    // Set port attributes and save old ones
    portAttributesHandler(fd);

    // instala rotina que atende interrupcao
    (void) signal(SIGALRM, atende);

    if (mode == RECEIVER) {
        if (llopenReceiver(fd) == -1) 
            return -1;
    }
        
    else if (mode == TRANSMITTER){
        if (llopenTransmitter(fd) == -1)
            return -1;
    }

    alarm(0);

    return fd;
}

int llclose(int fd)
{
    //TODO: Send close frame

    setOldPortAttributes(fd); 

    return 0;
}

int llread(int fd, char* buffer)
{   
    char bufferAux;
    char bufferAux2[10000];
    char* bufferPtr = bufferAux2;    
    
    int readSize = 0;
    int close = 0;
    // int detectedEndFlag = 0;

    while (!close) {
        while(readSize != 1) {
            readSize = read(fd, &bufferAux, 1);
        }

        // check if is frame starter flag
        if (bufferAux == FRAME_FLAG) {
            printSuccess("Starter flag detected!\n");
            printWarning("DEBUG \n");

            while(1) {
                *bufferPtr = bufferAux;
                bufferPtr++;
                read(fd, &bufferAux, 1); 
                if(bufferAux == FRAME_FLAG) {
                    *bufferPtr = bufferAux;
                    bufferPtr++;
                    read(fd, &bufferAux, 1); 
                    break;
                }               
            }        
            close = 1;              
        }
        else 
            continue;
    }

    printFrame(bufferAux2, 5);

    if (checkIfIsFrame(bufferAux2, FRAME_REJ0, 0)) {
        printf("is the samee!\n");
    }
    else 
        printf("Not the same! \n");
    
    
    // // tries to read frame
    // while(1) {
    //     read(fd,buffer,16);
    //     // buffer[res]='\0';             /* set end of string, so we can printf */
    //     // for (i = 0; i < res; i++) printf("%c", buffer[i]);
    //     // printf("\n");
    //     // if (buffer[0] == 'z');
    //     //printf("buffer: %c\n", *buffer);
    //     buffer += 16;
    // }

    return 0;
}

int llwrite(int fd, char* buffer, int length)
{
    int bytesWritten = 0; 

    // build frame

    //send frame
    bytesWritten = write(fd, FRAME_REJ0, FRAME_SUPERVISION_SIZE);
    printf("Bytes written = %d\n", bytesWritten);
    printFrame((char*) FRAME_REJ0, FRAME_SUPERVISION_SIZE);
    
    return length;
}
