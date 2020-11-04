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

