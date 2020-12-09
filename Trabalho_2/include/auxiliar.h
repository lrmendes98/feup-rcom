#pragma once
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <termios.h>

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
