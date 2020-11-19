#include "auxiliar.h"


int exportFile(char* path, char** content) 
{
	FILE *ptr_myfile;

	ptr_myfile=fopen(path,"wb");
	if (!ptr_myfile) {
		printf("Unable to open file!");
		return 1;
	}
		
	fwrite(*content, sizeof(*content), 1, ptr_myfile);
		
	fclose(ptr_myfile);
	return 0;
}

char* stringAdd(const char *s1, const char *s2)
{
  const size_t len1 = strlen(s1);
  const size_t len2 = strlen(s2);
  char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    
  memcpy(result, s1, len1);
  memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
  return result;
}

long getFileBinary(char* path, char** buffer)
{
  FILE *filePtr;
  filePtr = fopen(path,"rb");

	if (!filePtr) {
		printf("Unable to open file!\n");
		return 0;
  }

  long fs;
    
  fseek(filePtr, 0L, SEEK_END);
  fs = ftell(filePtr);
  *buffer = malloc(sizeof(char) * fs); 
  fseek(filePtr, 0L, SEEK_SET);

  fread(*buffer, 1, fs, filePtr);
		
	fclose(filePtr);

  return fs;
}

char* generateRdmBytestream(size_t num_bytes)
{
  char *stream = malloc(num_bytes);
  size_t i;

  for (i = 0; i < num_bytes; i++)
    stream[i] = rand();

  return stream;
}

char* printError(char* msg)
{
  printf("%s", stringAdd("\033[1;31m", msg));
  printf("\033[0m"); // reset normal color
  return 0;
}

char* printWarning(char* msg)
{
  printf("%s", stringAdd("\033[1;33m", msg));
  printf("\033[0m"); // reset normal color
  return 0;
}

char* printSuccess(char* msg)
{
  printf("%s", stringAdd("\033[1;32m", msg));
  printf("\033[0m"); // reset normal color
  return 0;
}

speed_t getBaudRate(int baud)
{
    switch (baud) {
    case 9600:
        return B9600;
    case 19200:
        return B19200;
    case 38400:
        return B38400;
    case 57600:
        return B57600;
    case 115200:
        return B115200;
    case 230400:
        return B230400;
    case 460800:
        return B460800;
    case 500000:
        return B500000;
    case 576000:
        return B576000;
    case 921600:
        return B921600;
    case 1000000:
        return B1000000;
    case 1152000:
        return B1152000;
    case 1500000:
        return B1500000;
    case 2000000:
        return B2000000;
    case 2500000:
        return B2500000;
    case 3000000:
        return B3000000;
    case 3500000:
        return B3500000;
    case 4000000:
        return B4000000;
    default:
        printError("Unrecognized baudrate!\n");
        exit(-1); 
        return -1;
    }
}