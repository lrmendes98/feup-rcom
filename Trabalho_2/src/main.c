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
        exit(-1);

    /* Get host information */
    struct hostent *host;
    if (get_host_info(&host, hostName))
        exit(-1);

    /* Abrir uma TCP socket */
    /* Ligar e comunicar com o servidor FTP */
    int serverSocket;
    char *serverIPAddress = inet_ntoa(*((struct in_addr *)host->h_addr));
    if (open_socket_and_connect_server(&serverSocket, serverIPAddress, SERVER_FTP_PORT, TRUE))
    {
        print_error("Error openning and connecting to server socket\n");
        exit(-1);
    }

    /* Mandar credenciais */
    if (send_credentials(serverSocket, userName, password))
    {
        print_error("Error sending credentials\n");
        exit(-1);
    }

    /* Mudar para modo passivo */
    int filePort;
    if (switch_passive_mode(serverSocket, &filePort))
    {
        print_error("Error switching to passive mode\n");
        exit(-1);
    }

    /* Open file socket */
    int fileSocket;
    if (open_socket_and_connect_server(&fileSocket, serverIPAddress, filePort, FALSE))
    {
        print_error("Error creating file socket\n");
        return 1;
    }

    /* Mandar retr command */
    if (send_retrieve_command(serverSocket, filePath))
    {
        print_error("Error sending retrieve command\n");
        return 1;
    }

    /* Download binario e criar ficheiro */
    if (receive_and_create_file(fileSocket, fileName))
    {
        print_error("Error getting file\n");
        return 1;
    }

    /* Fechar sockets e outras cenas */
    close(fileSocket);
    close(serverSocket);

    return 0;
}