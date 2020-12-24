#include "auxiliar.h"

int main(int argc, char *argv[])
{
    printf("\n");

    /* Placeholder for parsed arguments */
    struct LinkInfo linkInfo;
    if (validate_and_parse_arguments(argc, argv[1], &linkInfo))
        exit(-1);

    /* Get host information */
    struct hostent *host;
    if (get_host(&host, linkInfo.hostName))
        exit(-1);

    /* Abrir uma TCP socket */
    /* Ligar e comunicar com o servidor FTP */
    int serverSocket;
    char *serverIPAddress = inet_ntoa(*((struct in_addr *)host->h_addr));
    if (open_socket_and_connect_server(&serverSocket,
                                       serverIPAddress, FTP_PORT, TRUE))
    {
        print_error("Error openning and connecting to server socket\n");
        exit(-1);
    }

    /* Mandar credenciais */
    if (send_credentials(serverSocket, linkInfo.userName, linkInfo.password))
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
        exit(-1);
    }

    /* Mandar retr command */
    if (send_retrieve_command(serverSocket, linkInfo.fileName))
    {
        print_error("Error sending retrieve command \n");
        print_warning("File may not exist in FTP server\n");
        exit(-1);
    }

    /* Download binario e criar ficheiro */
    if (receive_and_create_file(fileSocket, linkInfo.fileName))
    {
        print_error("Error getting file\n");
        exit(-1);
    }

    /* Fechar FTP socket e file descriptor */
    close(fileSocket);
    close(serverSocket);

    return 0;
}