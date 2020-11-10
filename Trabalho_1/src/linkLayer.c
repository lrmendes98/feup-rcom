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
    int receivedFrameSize = -1;

    struct pollfd fds[1];
    fds[0].fd = fd;
    fds[0].events = 0;
    fds[0].events |= POLLIN;
    int retval;


    while (!close) {
        int readtries = 0;
        while(readSize != 1) {
            readtries++;            
            retval = poll(fds, 1, 0);
            if (retval != 0 && retval != -1 && fds[0].revents == POLLIN) 
                readSize = read(fd, &bufferAux, 1);
        }

        // check if incoming byte is frame starter flag
        if (bufferAux == FRAME_FLAG) {
            receivedFrameSize = 1;
            while(1) {
                receivedFrameSize++;
                *bufferPtr = bufferAux;
                bufferPtr++;
                retval = poll(fds, 1, 0);
                if (retval != 0 && retval != -1 && fds[0].revents == POLLIN) 
                    read(fd, &bufferAux, 1); 
                //printf("%X ", (u_int8_t)bufferAux);
                if(bufferAux == FRAME_FLAG) {
                    if (receivedFrameSize == 2) {
                        receivedFrameSize = 1;
                        bufferPtr--;
                        continue;
                    }

                    *bufferPtr = bufferAux;
                    bufferPtr++;
                    //read(fd, &bufferAux, 1); 
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

    return receivedFrameSize;
}

void atende()
{
    counterTries++;
    if (counterTries < MAXTRIES) {
        printWarning("Timeout #");
        printf("%i \n", counterTries);
    }  
}

void atendeReceiveFrame()
{
    counterTries++;
    if (counterTries < MAXTRIES) {
        printWarning("Timeout #");
        printf("%i \n", counterTries);
    }
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
    newtio.c_cc[VMIN] = 1;

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


        struct pollfd fds[1];
        fds[0].fd = fd;
        fds[0].events = 0;
        fds[0].events |= POLLIN;
        int retval;
        retval = poll(fds, 1, 0);

        if (retval != 0 && retval != -1 && fds[0].revents == POLLIN) {
            readSize = read(fd, &buffer, FRAME_SUPERVISION_SIZE);
            if (checkIfIsFrame(buffer, FRAME_UA, 0)) {
                //printSuccess("Received UA! \n");
                return 1;
            }
            else if (checkIfIsFrame(buffer, FRAME_REJ0, 0)) {
                printError("Received frame is REJ0! \n");
                return -1;
            }
            else {
                // printFrame(buffer, FRAME_SUPERVISION_SIZE);
                readSize = 0;
                // printError("Didn't recognize frame! \n");
            }
        }
        
        if (readSize == FRAME_SUPERVISION_SIZE) 
            break;
    }

    if (counterTries == MAXTRIES) {
        printError("Exceeded MAXTRIES!\n");
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
        struct pollfd fds[1];
        fds[0].fd = fd;
        fds[0].events = 0;
        fds[0].events |= POLLIN;
        int retval;
        retval = poll(fds, 1, 0);
        if (retval != 0 && retval != -1 && fds[0].revents == POLLIN)
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
    int bufferSize = 1000;
    char bufferAux[bufferSize];
    //int currentIndex = 1; // O recetor comeca com index 1
    //int test = 1;
    int close = 0;
    int receivedFrameSize = 0;
    static int index = 1; // o recetor comeca com index 1
    int receivedIndex = 0;
    int error = 0;

    while(!close) {

        // Receives Information frame
        receivedFrameSize = receiveFrame(fd, bufferAux);
        //printf("Received Bytes = %i \n", receivedFrameSize);

        if (receivedFrameSize != -1) {

            if (receivedFrameSize <= 5)
                receivedFrameSize = receiveFrame(fd, bufferAux);

            // remove flags
            char* bufferAuxPtr = bufferAux;
            bufferAuxPtr++;
            receivedFrameSize -= 2;

            int stuffed_flags = numberStuffedFlags(bufferAuxPtr, receivedFrameSize);

            char frame[receivedFrameSize - stuffed_flags];

            destuffing(bufferAuxPtr, receivedFrameSize, frame);

            receivedFrameSize -= stuffed_flags;

            // Checks if frame is correct
            if (unBuildFrame(frame, receivedFrameSize, receivedIndex, buffer) == -1)
                error = 1;
            
            // Check index
            // if (receivedIndex == index)
            //     error = 1;
            
            //TEST
            //sleep(6);

            if (error) {
                if (index == 0) 
                    write(fd, FRAME_REJ0, FRAME_SUPERVISION_SIZE);
                else
                    write(fd, FRAME_REJ1, FRAME_SUPERVISION_SIZE);
            }
            else {
                if (index == 0) 
                    write(fd, FRAME_RR0, FRAME_SUPERVISION_SIZE);
                else
                    write(fd, FRAME_RR1, FRAME_SUPERVISION_SIZE);
                close = 1;
            }            
        }
    }

    index ^= 1;

    return receivedFrameSize;
}

int llwrite(int fd, char* buffer, int length)
{

    char responseBuffer[FRAME_SUPERVISION_SIZE];

    int bytesWritten = 0; 
    static int sentFrameIndex = 0; // o emissor comeca com index 0
    
    // build frame without flags
    int frameLength = length + 4; // address, control, BCC1 and BCC2
    char unstuffed_frame[frameLength];

    if(!buildFrame(buffer, length, sentFrameIndex, unstuffed_frame)) {
        printError("Failed to build frame! \n");
        exit(-1);
    }

    int flags_to_stuff = numberFlagsToStuff(unstuffed_frame, frameLength);

    char frame[frameLength + flags_to_stuff];
    
    stuffing(unstuffed_frame, frameLength, frame);

    frameLength += flags_to_stuff;

    // insert flags
    bytesWritten = writeFrameWithFlags(fd, frame, frameLength);

    //TEST
    //sleep(1);

    int currentCount, readSize = 0;
    counterTries = 0;

    alarm(TIMEOUT); 

    while(counterTries < MAXTRIES) {
        if (currentCount != counterTries) {
            currentCount = counterTries;
            alarm(TIMEOUT);
            write(fd, FRAME_SET, FRAME_SUPERVISION_SIZE);
        }


        struct pollfd fds[1];
        fds[0].fd = fd;
        fds[0].events = 0;
        fds[0].events |= POLLIN;
        int retval;
        retval = poll(fds, 1, 0);

        if (retval != 0 && retval != -1 && fds[0].revents == POLLIN) {
            readSize = read(fd, &responseBuffer, FRAME_SUPERVISION_SIZE);
            // checks received frame index. Received response must be oposite index of send frame
            int receivedIndex = getFrameIndex(responseBuffer);
            if (receivedIndex != sentFrameIndex) {
                //printSuccess("Correct index! \n");
            }
            else {
                printWarning("Wrong index! \n");
                // resend frame
                bytesWritten = writeFrameWithFlags(fd, frame, frameLength);
                counterTries = 0;
                continue;
            }

            // check if received response is RR or REJ    
            if (checkIfIsFrame(responseBuffer, FRAME_RR0, 0) ||
                checkIfIsFrame(responseBuffer, FRAME_RR1, 0)) 
            {
                //printSuccess("Received RR\n");
                break;
            }
            else if (checkIfIsFrame(responseBuffer, FRAME_REJ0, 0) ||
                checkIfIsFrame(responseBuffer, FRAME_REJ1, 0)) 
            {
                printError("Received REJ\n");
                // resend frame
                bytesWritten = writeFrameWithFlags(fd, frame, frameLength);
                counterTries = 0;
                continue;
            }
            else {
                readSize = 0;
            }
        }
        
        if (readSize == FRAME_SUPERVISION_SIZE) 
            break;
    }

    if (counterTries == MAXTRIES) {
        printError("Exceeded MAXTRIES!\n");
        return -1;
    }

    // bytesWritten = write(fd, FRAME_REJ0, FRAME_SUPERVISION_SIZE);
    // printf("Bytes written = %d\n", bytesWritten);
    // printFrame((char*) FRAME_REJ0, FRAME_SUPERVISION_SIZE);

    sentFrameIndex ^= 1;
    
    return bytesWritten;
}
