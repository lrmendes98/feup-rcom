#include "linkLayer.h"


/* Global Variables */
struct termios oldtio;


int llopen(char* porta, int mode)
{
    int fd = open(porta, O_RDWR | O_NOCTTY);

    if (fd < 0) {
        printf("Error opening port! %s", porta);
        return -1;
    }

    // Set port attributes and save old ones
    getAndSaveOldPortAttributes(fd);

    if (mode == RECEIVER)
        llopenReceiver(fd);
    else if (mode == TRANSMITTER)
        llopenTransmitter(fd);

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
    struct termios newtio;
    
    /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
    */

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME] = 0;   /* inter-character timer unused */
    newtio.c_cc[VMIN] = 5;   /* blocking read until 5 chars received */

    /* 
    VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a 
    leitura do(s) proximo(s) caracter(es)
    */

    tcflush(fd, TCIOFLUSH);

    if (tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

    printSuccess("New termios structure set");
   
    return 0;
}

int llopenReceiver(int fd)
{
    struct termios newtio;

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME] = 0;   /* inter-character timer unused */
    newtio.c_cc[VMIN] = 5;   /* blocking read until 5 chars received */
    //newtio.c_cc[VMIN] = 0;

  /* VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a 
    leitura do(s) proximo(s) caracter(es)
    tcflush(fd, TCIOFLUSH);
  */

    if (tcsetattr(fd, TCSANOW, &newtio) == -1) {
      printError("tcsetattr has failed!");
      exit(-1);
    }
   
    printSuccess("New termios structure set");

    return 0;
}