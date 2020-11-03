#include "auxiliar.h"

char* stringAdd(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
    return result;
}

char* getFileBinary(char* path)
{
    FILE *filePtr;
    filePtr = fopen(path,"rb");

	if (!filePtr) {
		printf("Unable to open file!\n");
		return "";
    }

    long fs;
    
    fseek(filePtr, 0L, SEEK_END);
    char * buffer;
    fs = ftell(filePtr);
    buffer = malloc(sizeof(char) * fs); 
    fseek(filePtr, 0L, SEEK_SET);

    fread (buffer, 1, fs, filePtr);
		
	fclose(filePtr);

    return buffer;
}

char* generateRdmBytestream(size_t num_bytes)
{
  char *stream = malloc (num_bytes);
  size_t i;

  for (i = 0; i < num_bytes; i++)
  {
    stream[i] = rand ();
  }

  return stream;
}
