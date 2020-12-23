# RCOM Projeto 2
Autores:  
Filipe Reis Almeida (up201708999)  
Luís Ricardo Matos Mendes (up201604835)

## Sumário
Este trabalho aborda o desenvolvimento de uma aplicação que descarrega ficheiros de um servidor FTP. Esta aplicação tem de funcionar corretamente nos computadores tux3 e tux2 do laboratório que inicialmente não têm acesso à internet. Para tal, é necessário o setup devido destes computadores para que seja possível o download de um ficheiro de um servidor FTP externo.

## Introdução
O objetivo deste trabalho consiste em produzir uma aplicação cuja funcionalidade é executar a transferência de ficheiros de um servidor FTP e configurar uma network entre os computadores para que a partir destes seja possível o download de um ficheiro de um servidor FTP.  
 
Neste relatório está explicado a nossa implementação da aplicação, a sua arquitetura e forma de lidar com os erros, tal como a configuração da rede, análise dos logs e conclusões retiradas. 

## Aplicação download
### Arquitetura da aplicação
A estrutura global da aplicação, no que diz respeito ao envio dos comandos, é semelhante à experiência do telnet no guião da primeira aula.  
O primeiro passo é retirar a informação da ligação a partir do link inserido como argumento. O método validate_and_parse_arguments valida o número de argumentos inseridos e aloca para a struct LinkInfo o username, a password, o hostname e o filePath. Também lida com links sem username e password, nesse caso, o username e password enviados pelos comandos ao servidor ftp são "anonymous" e "1", respetivamente. Também lida com links mal construídos, retornando 1 caso algum elemento não exista ou a síntaxe do link esteja incorreta.  

De seguida, com método get_host obtém-se a informação do host a partir do node do host fornecido no link. Caso a função gethostbyname falhe, o programa retorna erro.  

De seguida, o programa abre uma socket TCP e connecta-se ao servidor. Esta socket é usada para o envio do user e da password (método send_credentials) e do comando pasv (método switch_passive_mode). Em ambos estes métodos, caso seja recebido um código de resposta diferente de 2xx e 3xx, o programa retorna com erro.  

No switch_passive_mode, o programa aguarda por uma resposta com código 230 que contém a string, por exemplo: "Entering Passive Mode (149,20,1,49,65,17)", da qual se retira a nova porta dos dois últimos códigos com a lógica 65*256 + 17 (método get_port). Como nos métodos anteriores, caso receba um código diferente de 2xx e 3xx, o programa retorna erro.
Com a porta recebida pela resposta do modo passivo, abre-se uma nova socket TCP associada ao endereço IP retirado pela gethostbyname.  

Depois, envia-se o comando retrieve pela socket utilizada para o envio dos comandos e de seguida faz-se a leitura da nova socket criada para a receção do ficheiro (método receive_and_create_file).  Caso o servidor FTP retorne um código diferente de 1xx no envio do comando retr, o programa termina com erro, pois é provável que o ficheiro não exista no servidor. Caso o read da socket FTP falhe, o programa termina com erro. Durante a leitura da socket é feita a escrita dos bytes para o file descriptor do ficheiro criado na raíz do projeto.  

Ao terminar, fecha-se as as duas sockets TCP criadas.


### Resultados de um download com sucesso

## Configuração da network e análise
> Por cada experiência (1 a 7):  
Network architecture,  experiment objectives, main configuration commands  
Analysis of the logs captured that are relevant for the learning objectives

## Conclusões