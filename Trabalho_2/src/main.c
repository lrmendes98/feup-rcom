#include "auxiliar.h"

int main(int argc, char *argv[])
{
    printf("\n");

    /* Placeholder for parsed arguments */
    char *userName = NULL;
    char *password = NULL;
    char *hostName = NULL;
    char *filePath = NULL;
    char *fileName = NULL;

    if (parse_arguments(argc, argv[1], &userName, &password, &hostName, &filePath, &fileName))
        return -1;

    printf("UserName = %s\nPassword = %s\nHostName = %s\nFilePath = %s\nFileName = %s\n", userName, password, hostName, filePath, fileName);

    /* Get host information */
    struct hostent *host;
    if (get_host_info(&host, hostName))
        return 1;

#ifdef DEBUG
    printf("Host name: %s\n", host->h_name);
    printf("IP Address: %s\n", inet_ntoa(*((struct in_addr *)host->h_addr)));
#endif

    /* Abrir uma TCP socket */
    /* Ligar e comunicar com o servidor FTP */
    int serverSocket;
    char *serverIPAddress = inet_ntoa(*((struct in_addr *)host->h_addr));
    if (open_socket_and_connect_server(&serverSocket, serverIPAddress, SERVER_FTP_PORT, TRUE))
    {
        print_error("Error openning and connecting to server socket\n");
        return 1;
    }

    /* Mandar credenciais */
    if(send_credentials(serverSocket, userName, password)) {
        print_error("Error sending credentials\n");
        exit(-1);
    }

    /* Mudar para modo passivo */

    /* Open file socket */

    /* Mandar retr command */

    /* Download binario e criar ficheiro */

    /* Fechar sockets e outras cenas */

    return 0;
}