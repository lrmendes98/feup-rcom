#pragma once
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <strings.h>
#include <signal.h>
#include <unistd.h>
#include "macros.h"

/* Placeholder for link parser information */
struct LinkInfo
{
    char *userName;
    char *password;
    char *hostName;
    char *filePath;
    char *fileName;
};

int receive_and_create_file(int socketFileDescriptor, char *fileName);

int send_retrieve_command(int socketFileDescriptor, char *filePath);

int get_port(int socketFileDescriptor, int *port);

int switch_passive_mode(int serverSocket, int *fileSocket);

int send_credentials(int socketFileDescriptor, char *userName, char *password);

int send_command(int socketFileDescriptor, char *command, char *argument);

int print_response_code(char *responseCode, int responseCodeSize);

int get_server_response(int socketFileDescriptor, char response[], int responseSize);

int open_socket_and_connect_server(int *socketFileDescriptor, char *serverAddress,
                                   int serverPort, int checkResponseCode);

int get_host(struct hostent **host, char *hostName);

/** Creates a file with received content (binary)
 * @param path: New file path
 * @param content: New file binary
*/
int validate_and_parse_arguments(int argc, char *argv, struct LinkInfo *linkInfo);

char *string_add(const char *s1, const char *s2);

char *print_error(char *msg);

char *print_warning(char *msg);

char *print_success(char *msg);
