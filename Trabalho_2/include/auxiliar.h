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

int receive_and_create_file(int socketFileDiscriptor, char *fileName);

int send_retrieve_command(int socketFileDiscriptor, char *filePath);

int get_port(int socketFileDiscriptor, int *port);

int switch_passive_mode(int serverSocket, int *fileSocket);

int send_credentials(int socketFileDiscriptor, char *userName, char *password);

int send_command(int socketFileDiscriptor, char *command, char *argument);

int print_response_code(char *responseCode, int responseCodeSize);

int get_server_response(int socketFileDiscriptor, char response[], int responseSize);

int open_socket_and_connect_server(int *socketFileDiscriptor, char *serverAddress,
                                   int serverPort, int checkResponseCode);

int get_host_info(struct hostent **host, char *hostName);

int parse_arguments(int argc, char *argv, char **username, char **password,
                    char **host, char **filePath, char **fileName);

char *stringAdd(const char *s1, const char *s2);

char *print_error(char *msg);

char *print_warning(char *msg);

char *print_success(char *msg);

/** Creates a file with received content (binary)
 * @param path: New file path
 * @param content: New file binary
*/
int exportFile(char *path, char **content);
