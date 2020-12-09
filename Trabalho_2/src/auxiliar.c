#include "auxiliar.h"

int parse_arguments(int argc, char *argv, char **username, char **password, char **host, char **filePath, char **fileName)
{
    if (argc != 2)
    {
        printf("Usage: ftp://<user>:<password>@<host>/<url-filePath>\n");
        exit(-1);
    }

    /* 
        ftp://<username>:<password>@<host>/<url-filePath>
        Exemplo: ftp://anonymous:1@speedtest.tele2.net/1KB.zip
    */   

    // Check if starting string "ftp://" is correct
    char *expectedStartingToken = "ftp://";
    int expectedStartingTokenSize = strlen(expectedStartingToken);
    if (strncmp(argv, expectedStartingToken, expectedStartingTokenSize))
    {
        fprintf(stderr, "Starting token was not the expected one\n");
        return 1;
    }
    argv += expectedStartingTokenSize; //Move pointer to parse remaining information

    // Parse username
    *username = strtok(argv, ":");

    // Parse password
    *password = strtok(NULL, "@");

    // Parse host
    *host = strtok(NULL, "/");

    // Parse filePath
    *filePath = strtok(NULL, "\0");

    // Isolate filename
    char *iterator;
    *fileName = NULL; //If value was not initially NULL, put it to NULL to make future error checking easier
    for (iterator = *filePath; *iterator != '\0'; iterator++)
        if (*iterator == '/') //Find each instant of char '/' and only save the last one
            *fileName = iterator;

    if (*fileName == NULL) //If no '/' is found than fileName is the same as filePath
        *fileName = *filePath;
    else
        (*fileName)++; // because of previous loop, fileName points to a '/' so we have to move it one character forward

    // Check if values are acceptable. Tests are done in oposite order because fileName is more likely to be corrupted than username
    if (*fileName == NULL || *filePath == NULL || *host == NULL || *password == NULL || *username == NULL)
    {
        print_error("Error parsing arguments\n");
        exit(-1);
    }

    return 0;
}

int exportFile(char *path, char **content)
{
    FILE *ptr_myfile;

    ptr_myfile = fopen(path, "wb");
    if (!ptr_myfile)
    {
        printf("Unable to open file!");
        return 1;
    }

    fwrite(*content, sizeof(*content), 1, ptr_myfile);

    fclose(ptr_myfile);
    return 0;
}

char *stringAdd(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
    return result;
}

char *print_error(char *msg)
{
    printf("%s", stringAdd("\033[1;31m", msg));
    printf("\033[0m"); // reset normal color
    return 0;
}

char *print_warning(char *msg)
{
    printf("%s", stringAdd("\033[1;33m", msg));
    printf("\033[0m"); // reset normal color
    return 0;
}

char *print_success(char *msg)
{
    printf("%s", stringAdd("\033[1;32m", msg));
    printf("\033[0m"); // reset normal color
    return 0;
}
