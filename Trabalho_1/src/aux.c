#include "aux.h"

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
    char buffer[999999];
    FILE *filePtr;
    filePtr = fopen(path,"rb");

	if (!filePtr) {
		printf("Unable to open file!");
		return "";
    }
		
	fread(&buffer,sizeof(buffer),1,filePtr);
		
	fclose(filePtr);

    printf("%s", buffer);

    return buffer;
}