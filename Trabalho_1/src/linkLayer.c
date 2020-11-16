#include "linkLayer.h"

struct termios oldtio;
unsigned int counterTries = 0;
int frameTimout = 0;

int receiveFrame(int fd, char* buffer)
{
    char bufferAux;
    char* bufferPtr = buffer;    
    
    int readSize = 0;
    int close = 0;
    int receivedFrameSize = -1;

    /*struct pollfd fds[1];
    fds[0].fd = fd;
    fds[0].events = 0;
    fds[0].events |= POLLIN;
    int retval;*/

    while (!close) {
		if (frameTimout){
			frameTimout = 0;
			return 0;
		}
        int readtries = 0;
        while(readSize != 1) {
			if (frameTimout){
				frameTimout = 0;
				return 0;
			}
            readtries++;            
            //retval = poll(fds, 1, 0);
            //if (retval != 0 && retval != -1 && fds[0].revents == POLLIN) {
            readSize = read(fd, &bufferAux, 1);
            //}
        }

        // check if incoming byte is frame starter flag
        if (bufferAux == FRAME_FLAG) {
            *bufferPtr = FRAME_FLAG; 
            receivedFrameSize = 1;
            while(1) {
				if (frameTimout){
					frameTimout = 0;
					return 0;
				}
                //retval = poll(fds, 1, 0);
                //if (retval != 0 && retval != -1 && fds[0].revents == POLLIN) {
                readSize = read(fd, &bufferAux, 1); 
                if (readSize != 0) {
                    receivedFrameSize++;
                    bufferPtr++;
                    *bufferPtr = bufferAux;
                }
                //}
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

void atendeReceiveNotFrame()
{
    printWarning("Read Timeout\n");
    frameTimout = 1;
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
    newtio.c_cflag = baudrate | CS8 | CLOCAL | CREAD;
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

int llcloseTransmitter(int fd)
{
    // Send DISC
    printWarning("Disconecting . . .\n");
    write(fd, FRAME_DISC, FRAME_SUPERVISION_SIZE);

    // Receive DISC
    char bufferAux[FRAME_SUPERVISION_SIZE];
    receiveFrame(fd, bufferAux);

    if (checkIfIsFrame(bufferAux, FRAME_DISC, 0)) {
        write(fd, FRAME_UA, FRAME_SUPERVISION_SIZE);
        printSuccess("Terminated with Success!\n\n");

        return 0;
    }
    
    return 1;
}

int llcloseReceiver(int fd)
{
    printWarning("Disconnecting . . .\n");

    int close = 0;
    while(!close) {
        // Receive DISC
        char bufferAux[FRAME_SUPERVISION_SIZE];
        receiveFrame(fd, bufferAux);

        if (checkIfIsFrame(bufferAux, FRAME_DISC, 0)) {
            // Send DISC
            write(fd, FRAME_DISC, FRAME_SUPERVISION_SIZE);

            // Receive UA
            receiveFrame(fd, bufferAux);

            if (checkIfIsFrame(bufferAux, FRAME_UA, 0)) {
                printSuccess("Terminated with Success!\n\n");
                return 0;
            }
        }
        else {
            continue;
        }
    }

    return 1;
}

int llopenTransmitter(int fd)
{
    int currentCount, readSize = 0;
    char buffer[FRAME_SUPERVISION_SIZE];

    write(fd, FRAME_SET, FRAME_SUPERVISION_SIZE);

    alarm(timeoutSeconds);    
    
    while(counterTries < MAXTRIES) {
        if (currentCount != counterTries) {
            currentCount = counterTries;
            alarm(timeoutSeconds);
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
    setOldPortAttributes(fd); 

    return 0;
}

int llread(int fd, char* buffer)
{   
    int bufferSize = (packetSize * 2) + 20;
    char bufferAux[bufferSize];
    int close = 0;
    int receivedFrameSize = 0;
    static int index = 1; // o recetor comeca com index 1
    int receivedIndex = 0;
    int error = 0;
    
    (void) signal(SIGALRM, atendeReceiveNotFrame);

    while(!close) {

        // Receives Information frame
        int readTimeout = 2;
        if (timeoutSeconds >= 4)
			readTimeout = timeoutSeconds - 2;
        alarm(readTimeout);
        receivedFrameSize = receiveFrame(fd, bufferAux);
        alarm(0);

        if (receivedFrameSize != -1) {
			if (receivedFrameSize == 0) {
				continue;
			}

            if (receivedFrameSize <= 5)
                continue;

            // remove flags
            char* bufferAuxPtr = bufferAux;
            bufferAuxPtr++;
            receivedFrameSize -= 2;

            char frame[packetSize + 50];
            char bcc2 = destuffing(bufferAuxPtr, &receivedFrameSize, frame, buffer);

            //check index
            u_int8_t controlField = frame[1]; 
            if (controlField == FRAME_CONTROL_FIELD_SEND0) 
                receivedIndex = 0;
            else if (controlField == FRAME_CONTROL_FIELD_SEND1)
                receivedIndex = 1;

            if (receivedIndex == index) {
                printWarning("Wrong index!\n");
                if (receivedIndex == 0) 
                    write(fd, FRAME_RR1, FRAME_SUPERVISION_SIZE);
                else
                    write(fd, FRAME_RR0, FRAME_SUPERVISION_SIZE);
                continue;
            }
            
            // Checks if frame is correct
            if (unBuildFrame(frame, receivedFrameSize, buffer, bcc2) == -1)
                error = 1;
           
            if (error) {
                error = 0;
                if (index == 0) 
                    write(fd, FRAME_REJ0, FRAME_SUPERVISION_SIZE);
                else
                    write(fd, FRAME_REJ1, FRAME_SUPERVISION_SIZE);
            }
            else {
				//TEST
            	//sleep(3);
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
    int frameLength = (packetSize + 8) * 2; // address, control, BCC1 and BCC2
    char frame[frameLength];

    if(!buildFrame(buffer, &length, sentFrameIndex, frame)) {
        printError("Failed to build frame! \n");
        exit(-1);
    }        

    // insert flags
    bytesWritten = writeFrameWithFlags(fd, frame, length);

    //TEST
    //sleep(1);

    int currentCount = 0;
    counterTries = 0;

    alarm(timeoutSeconds); 

    while(counterTries < MAXTRIES) {
        if (currentCount != counterTries) {
            currentCount = counterTries;
            alarm(timeoutSeconds);
            bytesWritten = writeFrameWithFlags(fd, frame, length);
        }

        struct pollfd fds[1];
        fds[0].fd = fd;
        fds[0].events = 0;
        fds[0].events |= POLLIN;
        int retval;
        retval = poll(fds, 1, 0);

        if (retval != 0 && retval != -1 && fds[0].revents == POLLIN) {
            read(fd, &responseBuffer, FRAME_SUPERVISION_SIZE);
            // checks received frame index. Received response must be oposite index of send frame
            int receivedIndex = getFrameIndex(responseBuffer);
            if (receivedIndex != sentFrameIndex) {
                //printSuccess("Correct index! \n");
            }
            else {
                printWarning("Wrong index! \n");
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
                bytesWritten = writeFrameWithFlags(fd, frame, length);
                counterTries = 0;
                continue;
            }
            else {}
        }
    }

    if (counterTries == MAXTRIES) {
        printError("Exceeded MAXTRIES!\n");
        return -1;
    }

    sentFrameIndex ^= 1;
    
    return bytesWritten;
}
