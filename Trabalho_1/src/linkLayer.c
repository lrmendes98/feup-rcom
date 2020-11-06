#include "linkLayer.h"


/* Global Variables */
struct termios oldtio;
unsigned int counterTries = 0;


int receiveFrame(int fd, char* buffer)
{
    char bufferAux;
    char* bufferPtr = buffer;    
    
    int readSize = 0;
    int close = 0;

    while (!close) {
        while(readSize != 1) {
            readSize = read(fd, &bufferAux, 1);
        }

        // check if incoming byte is frame starter flag
        if (bufferAux == FRAME_FLAG) {
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

                // TODO: Caso não receba, fazer qualquer coisa com timeouts              
            }        
            close = 1;              
        }

        else 
            continue;
    }

    //printFrame(buffer, 5);

    // if (checkIfIsFrame(buffer, FRAME_REJ0, 0)) {
    //     printf("is the samee!\n");
    // }
    // else 
    //     printf("Not the same! \n");

    return 1;
}

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
    int bufferSize = 20;
    char bufferAux[bufferSize];
    //int currentIndex = 1; // O recetor comeca com index 1

    // Receives Information frame
    if (receiveFrame(fd, bufferAux)) {
        printf("Received frame \n");

        // Writes response RR
        write(fd, FRAME_RR1, FRAME_SUPERVISION_SIZE);
    }

    return 0;
}

int llwrite(int fd, char* buffer, int length)
{
    char responseBuffer[FRAME_SUPERVISION_SIZE];

    int bytesWritten = 0; 
    int close = 0;
    int sentFrameIndex = 0; // O recetor comeca com index 0

    // build frame, frame stuffing...

    //send frame
    bytesWritten = write(fd, FRAME_REJ0, FRAME_SUPERVISION_SIZE);
    printf("Bytes written = %d\n", bytesWritten);

    while(!close) {
        // waits for answer
        if (receiveFrame(fd, responseBuffer)) {
            
            // check received frame index. Received response must be oposite index of send frame
            if (checkFrameIndex(responseBuffer, sentFrameIndex)) {
                printSuccess("Correct index! \n");
            }
            else {
                printWarning("Wrong index! \n");
            }
            
               

            close = 1;            
        }
        else {
            // if receiveFrame has failed, send REJ frame
        } 
    }

    // bytesWritten = write(fd, FRAME_REJ0, FRAME_SUPERVISION_SIZE);
    // printf("Bytes written = %d\n", bytesWritten);
    // printFrame((char*) FRAME_REJ0, FRAME_SUPERVISION_SIZE);
    
    return length;
}
