#include "auxiliar.h"

int receive_and_create_file(int socketFileDiscriptor, char *fileName)
{
    FILE *file = fopen(fileName, "w+");
    if (file == NULL)
        return 1;

    char buffer;
    while (read(socketFileDiscriptor, &buffer, 1))
        fwrite(&buffer, sizeof(buffer), 1, file);

    if (fclose(file))
        return 1;

    print_success("Finished file Download\n\n");

    return 0;
}

int send_retrieve_command(int socketFileDiscriptor, char *filePath)
{
    char responseCode[RESPONSE_CODE_SIZE];

    printf("> retr %s\n", filePath);
    if (send_command(socketFileDiscriptor, "retr ", filePath))
    {
        fprintf(stderr, "Error sending \'retr\' command\n");
        return 1;
    }
    if (get_server_response(socketFileDiscriptor, responseCode, RESPONSE_CODE_SIZE))
    {
        fprintf(stderr, "Error getting retrieve response code\n");
        return 1;
    }
    print_response_code(responseCode, RESPONSE_CODE_SIZE);
    if (responseCode[0] != '1')
    {
        fprintf(stderr, "Unexpected response code at retrieve\n");
        return 1;
    }

    return 0;
}

int get_port(int socketFileDiscriptor, int *port)
{
    int firstByte = 0;
    int secoundByte = 0;
    // Expected to read response like: Entering Passive Mode (90,130,70,73,86,26)'
    char tempChar;
    do
    {
        read(socketFileDiscriptor, &tempChar, 1);
        putchar(tempChar);
    } while (tempChar != '(');

    int commaCounter = 0;
    while (commaCounter != 4)
    {
        read(socketFileDiscriptor, &tempChar, 1);
        putchar(tempChar);
        if (tempChar == ',')
            commaCounter++;
    }

    //Read the 3 chars of first byte
    tempChar = 0;
    int counter; // Keep track of how many chars have been read
    for (counter = 0; counter < 4; counter++)
    {
        read(socketFileDiscriptor, &tempChar, 1);
        putchar(tempChar);
        if (tempChar == ',')
            break;
        firstByte *= 10;
        firstByte += (tempChar - '0');
    }

    //Read the 3 chars of secound byte
    tempChar = 0;
    for (counter = 0; counter < 4; counter++)
    {
        read(socketFileDiscriptor, &tempChar, 1);
        putchar(tempChar);
        if (tempChar == ')')
            break;
        secoundByte *= 10;
        secoundByte += (tempChar - '0');
    }
    putchar('\n');

    *port = firstByte * 256 + secoundByte;

    return 0;
}

int switch_passive_mode(int serverSocket, int *fileSocket)
{
    char responseCode[RESPONSE_CODE_SIZE];

    printf("> pasv\n");
    if (send_command(serverSocket, "pasv", NULL))
    {
        print_error("Error switching to passive mode\n");
        return 1;
    }

    // Get response code
    if (get_server_response(serverSocket, responseCode, RESPONSE_CODE_SIZE))
    {
        print_error("Error getting passive response code\n");
        return 1;
    }
    print_response_code(responseCode, RESPONSE_CODE_SIZE);

    if (responseCode[0] != '2')
    {
        print_error("Error switching to passive mode\n");
        return 1;
    }

    // Get new port to passive receive file
    if (get_port(serverSocket, fileSocket))
    {
        print_error("Error getting new port\n");
        return 1;
    }

    return 0;
}

int send_credentials(int socketFileDiscriptor, char *userName, char *password)
{
    char responseCode[RESPONSE_CODE_SIZE];

    /* Send username */
    printf("> user %s\n", userName);
    if (send_command(socketFileDiscriptor, "USER ", userName))
    {
        print_error("Error sending USER command\n");
        return 1;
    }
    if (get_server_response(socketFileDiscriptor, responseCode, RESPONSE_CODE_SIZE))
    {
        print_error("Error getting username response code\n");
        return 1;
    }
    print_response_code(responseCode, RESPONSE_CODE_SIZE);
    if (responseCode[0] != '3' && responseCode[0] != '2')
    {
        print_error("Unexpected response code at username\n");
        return 1;
    }

    /* Send password */
    printf("> pass %s\n", password);
    if (send_command(socketFileDiscriptor, "PASS ", password))
    {
        print_error("Error sending PASS command\n");
        return 1;
    }
    if (get_server_response(socketFileDiscriptor, responseCode, RESPONSE_CODE_SIZE))
    {
        print_error("Error getting password response code\n");
        return 1;
    }
    print_response_code(responseCode, RESPONSE_CODE_SIZE);
    if (responseCode[0] != '2')
    {
        print_error("Unexpected response code at password\n");
        return 1;
    }

    return 0;
}

int send_command(int socketFileDiscriptor, char *command, char *argument)
{
    if (!write(socketFileDiscriptor, command, strlen(command)))
        return 1;

    if (argument != NULL)
        if (!write(socketFileDiscriptor, argument, strlen(argument)))
            return 1;

    if (!write(socketFileDiscriptor, "\n", 1))
        return 1;

    return 0;
}

int print_response_code(char *responseCode, int responseCodeSize)
{
    if (responseCode == NULL || responseCodeSize <= 0)
        return 1;

    printf("< Response Code: ");
    int iter = 0;
    for (iter = 0; iter < responseCodeSize; iter++)
        printf("%c", responseCode[iter]);
    printf("\n");

    return 0;
}

int get_server_response(int socketFileDiscriptor, char response[], int responseSize)
{
    char tempChar;
    int responseIndex = 0;
    while (responseIndex < responseSize)
    {
        if (!read(socketFileDiscriptor, &tempChar, 1))
            return 1;

        if (tempChar < '0' || tempChar > '9')
            continue;
        response[responseIndex] = tempChar;
        responseIndex++;
    }
    return 0;
}

int open_socket_and_connect_server(int *socketFileDiscriptor, char *serverAddress, int serverPort, int checkResponseCode)
{
    struct sockaddr_in server_addr;

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(serverAddress); /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(serverPort);               /*server TCP port must be network byte ordered */

    /* open TCP socket */
    if ((*socketFileDiscriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        print_error("Error creating socket!\n");
        return 1;
    }

    /* connect to the server */
    print_warning("Connecting to server\n");
    if (connect(*socketFileDiscriptor, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0)
    {
        print_error("Error connecting to server!\n");
        return 1;
    }

    if (checkResponseCode)
    {
        char responseCode[RESPONSE_CODE_SIZE];

        /* bug ? Diferences in response schema of diferent RFC's */
        // if (get_server_response(*socketFileDiscriptor, responseCode, RESPONSE_CODE_SIZE))
        //     return 1;

        print_response_code(responseCode, RESPONSE_CODE_SIZE);

        if (get_server_response(*socketFileDiscriptor, responseCode, RESPONSE_CODE_SIZE))
        {
            print_error("Error getting username response code\n");
            return 1;
        }

        print_response_code(responseCode, RESPONSE_CODE_SIZE);
    }

    return 0;
}

int get_host_info(struct hostent **host, char *hostName)
{
    if (host == NULL)
        return 1;

    /*
    struct hostent {
        char *h_name;	    Official name of the host. 
        char **h_aliases;   A NULL-terminated array of alternate names for the host. 
        int h_addrtype;	    The type of address being returned; usually AF_INET.
        int h_length;	    The length of the address in bytes.
        char **h_addr_list;	A zero-terminated array of network addresses for the host. 
                    Host addresses are in Network Byte Order. 
    };

    #define h_addr h_addr_list[0]	The first address in h_addr_list. 
    */

    *host = gethostbyname(hostName);
    if (host == NULL)
    {
        herror("gethostbyname");
        exit(-1);
    }

    return 0;
}

int parse_arguments(int argc, char *argv, char **username, char **password, char **host, char **filePath, char **fileName)
{
    if (argc != 2)
    {
        printf("Usage: ftp://<user>:<password>@<host>/<url-filePath>\n");
        exit(-1);
    }

    /* 
        ftp://<username>:<password>@<host>/<url-filePath>
        Exemplo: 
        ftp://anonymous:1@speedtest.tele2.net/1KB.zip
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

    // Check if has username and password
    char *dpPointer = strchr(argv, ':');
    if (dpPointer == NULL)
    {
        printf("is null\n");
        *username = "anonymous";
        *password = "1";

        // Parse host
        *host = strtok(argv, "/");
    }
    else
    {
        // Parse username
        *username = strtok(argv, ":");

        // Parse password
        *password = strtok(NULL, "@");

        // Parse host
        *host = strtok(NULL, "/");
        printf("%s", *host);
    }

    // Parse filePath
    *filePath = strtok(NULL, "\0");

    // Isolate filename
    char *iterator;
    *fileName = NULL;
    for (iterator = *filePath; *iterator != '\0'; iterator++)
        if (*iterator == '/') // Find each instant of char '/' and only save the last one
            *fileName = iterator;

    if (*fileName == NULL) // If no '/' is found than fileName is the same as filePath
        *fileName = *filePath;
    else
        (*fileName)++;

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