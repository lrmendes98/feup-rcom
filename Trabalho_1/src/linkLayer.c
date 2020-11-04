#include "linkLayer.h"


/* Global Variables */
struct termios oldtio;
unsigned int counterTries = 0;
bool hasTimedOut = false;

void atende()
{
    printWarning("Timeout #");
    counterTries++;
    printf("%i \n", counterTries);
}

int setOldPortAttributes(int fd) {
    if (tcsetattr(fd,TCSANOW,&oldtio) == -1) {
      printError("tcsetattr has failed!");
      exit(-1);
    }
    return 1;
}

int getAndSaveOldPortAttributes(int fd)
{
    // save current port settings
    if (tcgetattr(fd, &oldtio) == -1) { 
      perror("tcgetattr has failed!");
      exit(-1);
    }

    return 1;
}

int llopenTransmitter(int fd)
{
    int currentCount = 0;
    char buffer[FRAME_SUPERVISION_SIZE];

    write(fd, FRAME_SET, FRAME_SUPERVISION_SIZE);

    alarm(TIMEOUT);    
    
    while(counterTries < MAXTRIES) {

        if (currentCount != counterTries) {
            currentCount = counterTries;
            alarm(TIMEOUT);
            write(fd, FRAME_SET, FRAME_SUPERVISION_SIZE);
        }
        
        int readSize = read(fd, &buffer, FRAME_SUPERVISION_SIZE);
        if (readSize == FRAME_SUPERVISION_SIZE) {
            break;
        }

    }

    if (counterTries == MAXTRIES) {
        printError("Exceeded MAXTRIES!\n");
        return -1;
    }

    printSuccess("has exited successfully!\n");    
   
    return 0;
}

int llopenReceiver(int fd)
{
    // read frames
    char buffer[sizeof(FRAME_SET)];
    
    int readSize = 0;

    while(readSize != FRAME_SUPERVISION_SIZE) {
        readSize = read(fd, &buffer, sizeof(FRAME_SET));
    }
    
    if (strcmp(buffer, (const char *)FRAME_SET) == 0) {
        printSuccess("Received SET");
        // Send UA
        write(fd, FRAME_UA, sizeof(FRAME_UA));        
    }
    else {
        printError("Rejected Link!");
        write(fd, FRAME_REJ0, sizeof(FRAME_REJ0));
    }

    //if frame == SET frame, send ACK frame, else, send REJ frame

    return 0;
}

int llopen(char* porta, int mode)
{
    int fd = open(porta, O_RDWR | O_NOCTTY);

    if (fd < 0) {
        printf("Error opening port! %s", porta);
        return -1;
    }

    // Set port attributes and save old ones
    getAndSaveOldPortAttributes(fd);

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
      perror("tcsetattr");
      exit(-1);
    }

    printSuccess("New termios structure set\n");

    (void) signal(SIGALRM, atende);  // instala rotina que atende interrupcao

    if (mode == RECEIVER)
        llopenReceiver(fd);
    else if (mode == TRANSMITTER)
        llopenTransmitter(fd);

    alarm(0);

    return fd;
}

int llclose(int fd)
{

    return 0;
}

int llread(int fd, char* buffer)
{   
    while(1) {
        read(fd,buffer,16);
        // buffer[res]='\0';             /* set end of string, so we can printf */
        // for (i = 0; i < res; i++) printf("%c", buffer[i]);
        // printf("\n");
        // if (buffer[0] == 'z');
        //printf("buffer: %c\n", *buffer);
        buffer += 16;
    }

    return 0;
}

int llwrite(int fd, char* buffer, int length)
{
    int bytes = 0;
      
    bytes = write(fd, buffer, length);
    printf("Bytes written = %d\n", bytes);
    
    return length;
}
