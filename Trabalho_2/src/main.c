#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "auxiliar.h"


int main(int argc, char *argv[])
{
    printf("Hello\n");

    /* Parse arguments */
    char *userName = NULL;
    char *password = NULL;
    char *hostName = NULL;
    char *filePath = NULL;
    char *fileName = NULL;

    if (parse_arguments(argc, argv[1], &userName, &password, &hostName, &filePath, &fileName))
        return -1;

    /* Get host information */

    /* Abrir uma TCP socket? */

    /* Ligar e comunicar com o servidor FTP */

    /* Mandar credenciais */

    /* Mudar para modo passivo */

    /* Open file socket */

    /* Mandar retr command */

    /* Download binario e criar ficheiro */

    /* Fechar sockets e outras cenas */

    return 0;
}