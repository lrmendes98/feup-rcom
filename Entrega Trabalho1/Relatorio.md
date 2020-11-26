# RCOM Projeto 1
Autores:  
Filipe Reis Almeida (up201708999)  
Luís Ricardo Matos Mendes (up201604835)

## Sumário
> (dois parágrafos: um sobre o contexto do trabalho; outro sobre as principais conclusões do relatório)

Este trabalho aborda a comunicação entre dois computadores via porta série. A realização deste projeto consiste no desenvolvimento de um programa cujo ambiente de execução é um terminal em Linux.  
Conclusões do relatório:


## Introdução
 > (indicação dos objectivos do trabalho e do relatório; descrição da lógica do relatório com indicações sobre o tipo de informação que poderá ser encontrada em cada uma secções seguintes)  

 O objetivo deste trabalho consiste em produzir uma aplicação cuja funcionalidade é executar a transferência de ficheiros entre um cliente e um servidor, conectados pela porta série. Esta aplicação tem de suportar interrupções de rede, corrupção de pacotes e a disconexão súbita do recetor.  
 > objetivo do relatorio???

 Nas secções que se seguem estará explícita a arquitetura do projeto, tanto como do código como da lógica do protocolo.


## Arquitetura 
> (blocos funcionais e interfaces)    

O código está organizado em 5 diretórios diferentes. Os diretórios 'src' e 'include' contém os ficheiros de código fonte e os ficheiros de cabeçalho, respetivamente.
No diretório 'obj' encontram-se os objetos resultantes da compilação e o próprio diretório é resultante da compilação do código. O diretório 'testFiles' contém os ficheiros utilizados para testar o protocolo e o 'copiedFiles' onde os ficheiros são recebidos depois do envio. O Makefile e os executáveis encontram-se na raíz da pasta.  

O nosso projeto é dividido em duas partes essenciais, a camada de aplicação (appLayer.c) e a camada de ligação (linkLayer.c). No ficheiro auxiliar.c estão contidas métodos auxiliares comuns entre a camada de ligação e a camada de aplicação. No ficheiro frame.c e o seu cabeçalho estão contidas as tramas de supervisão em forma de macros e outros métodos relacionados com o processamento de tramas. No ficheiro macros.h estão a maioria das  macros utilizadas neste projeto como o baudrate, macros que distinguem o recetor do transmissor e os bytes das tramas (bytes de controlo, flags, etc).


## Estrutura do código
> (APIs, principais estruturas de dados, principais funções e sua relação com a arquitetura)

A camada de ligação contém as seguintes funções principais:
- llopen
- llread
- llwrite  

Também foram criadas as funções llopenReceiver, llopenTransmitter, llcloseTransmitter e llcloseReceiver. Estas executam o protocolo como descrito com as respetivas diferenças caso seja o cliente ou o servidor.  

A camada de aplicação contém as seguintes funções principais:
- appLayerWrite
- appLayerRead  

No ficheiro macros.h está defenido o Baudrate, as macros usadas para distinguir o modo do programa (transmissor ou recetor), as diversas constantes utilizadas nas tramas de supervisão para o campo de endereço, campo de controlo (SET, DISC, UA, RR, REJ) e os respetivos índices.  

A camada de aplicação contém as seguintes funções principais:
- appLayerWrite
- appLayerRead

## Casos de uso principais

O programa é chamado da seguinte forma:
- Caso seja read: ```./read <port> <packetSize>```
- Caso seja write: ```./write <port> <fileName> <packetSize> <timeoutSeconds> <maxTries>```

Exemplos de execução do programa:
- ```./read /dev/ttyS0 2000```
- ```./write /dev/ttyS0 testFiles/pinguim.gif 2000 3 3```

O baudrate é defenido no ficheiro macros.h localizado na pasta include.

Sequência de chamada de funções:  

```./read /dev/ttyS0 2000```  
llopen -> llopenReceiver -> appLayerRead -> llread -> unBuildFrame -> appLayerRead (segue o mesmo ciclo até receber todos os pacotes) -> llcloseReceiver.  

```./write /dev/ttyS0 testFiles/pinguim.gif 2000 3 3```  
llopen -> llopenTransmitter -> appLayerWrite -> llwrite -> buildFrame -> appLayerWrite (segue o mesmo ciclo até enviar todos os pacotes) -> llcloseTransmitter.


## Protocolo de ligação lógica

### llopen
O protocolo de ligação inicia-se no main com a execução do llopen que gera um file descriptor com a porta introduzida e establece os atributos selecionados à porta série. Existe um  método dedicado apenas a este processo, o portAttributesHandler. No nosso projeto, defenimos o VMIN e o VTIME ambos a 0. De seguida instala-se a rotina que atende as interrupções via SIGALRM. O llopen recebe como argumento o modo do programa, seja este o transmissor ou o recetor. Caso seja o transmissor, segue para o llopenTransmitter, que executa o protocolo como descrito no guião da parte do transmissor. O mesmo se aplica para caso seja o recetor, executando o llopenReceiver.  

```c
int llopen(char* porta, int mode)
{
    int fd = open(porta, O_RDWR | O_NOCTTY);

    if (fd < 0) {
        printf("Error opening port! %s", porta);
        return -1;
    }

    // Set port attributes
    portAttributesHandler(fd);

    // Instala rotina que atende interrupcao
    (void) signal(SIGALRM, atende);

    if (mode == RECEIVER) {
        if (llopenReceiver(fd) == -1) 
            return -1;
    }
    else if (mode == TRANSMITTER) {
        if (llopenTransmitter(fd) == -1)
            return -1;
    }

    alarm(0);

    return fd;
}
```

### llread

O llread aguarda o retorno da receiveFrame em loop, o método responsável pela receção da trama. Establecemos o buffer de receção de bytes como o maior possível de acordo com o tamanho da trama introduzido na chamada do programa que é o tamanho da trama multiplicado por 2, no caso improvável de todo o pacote ser composto por bytes de flag. Deste modo excluímos o uso de malloc e realloc, aumentando a rapidez de execução do programa. Após a receção da trama, retira-se as flags de início e fim de trama, faz-se o destuffing da trama, que simultaneamente calcula e retorna o bcc2. De seguida, verifica se o índice da trama recebida é o correto. No caso do recetor (llread), o índice começa com o valor 1 e alterna sempre no fim da execução do llread.
```c
   static int index = 1; // o recetor comeca com index 1
   (...)
   index ^= 1;
```
Caso a trama recebida não contenha o índice oposto do recetor na altura da receção, significa que a trama recebida está dessincronizada e envia-se a trama de rejeição.
```c
   if (receivedIndex == index) {
      printWarning("Wrong index!\n");
      if (receivedIndex == 0) 
         write(fd, FRAME_RR1, FRAME_SUPERVISION_SIZE);
      else
         write(fd, FRAME_RR0, FRAME_SUPERVISION_SIZE);
      continue;
   }
```

De seguida, faz-se a desconstrução da trama no método unBuildFrame, que recebe a trama , o tamanho e o bcc2 calculado anteriormente. Este método é responsável pela verificação dos bcc's
```c
   u_int8_t bcc1 = frame[2];

   char correctBcc1 = frame[0] ^ frame[1];

   if (correctBcc1 != bcc1) {
      printError("Incorrect BCC1! \n");
      return -1;
   }
    
   char correctBcc2 = frame[frameLength - 1];
    
   if (correctBcc2 != bcc2) {
      printError("Incorrect bcc2! \n");
      return -1;
   }    
```
Caso seja detetado um erro, envia-se a respetiva trama de rejeição e retoma-se o início do loop. Caso não haja erro, fecha-se o loop de receção, altera-se o íncide do recetor e retorna o receivedFrameSize.
```c
   if (unBuildFrame(frame, receivedFrameSize, buffer, bcc2) == -1)
      error = 1;
```
```c
   if (error) {
      error = 0;
      if (index == 0) 
         write(fd, FRAME_REJ0, FRAME_SUPERVISION_SIZE);
      else
         write(fd, FRAME_REJ1, FRAME_SUPERVISION_SIZE);
   }
   else {
      if (index == 0) 
         write(fd, FRAME_RR0, FRAME_SUPERVISION_SIZE);
      else
         write(fd, FRAME_RR1, FRAME_SUPERVISION_SIZE);
      close = 1;
   }   
```
### llwrite
O llwrite é responsável por enviar a trama e responder ao recetor de acordo com a mensagem de erro que recebe. No início é chamada o método buildFrame que recebe o pacote que lhe é passado como argumento na appLayer, o tamanho desse pacote, o index atual do recetor e um buffer onde é colocada a trama construida. O index é calculado da mesma forma que o llread.
```c
   if(!buildFrame(buffer, &length, sentFrameIndex, frame)) {
      printError("Failed to build frame! \n");
      exit(-1);
   }   
```
Logo de seguida, é enviada essa trama para a porta série. Como o método buildFrame não insere as Flags iniciais e finais da trama, o método writeFrameWithFlags escreve para a porta série sequência FLAG -> trama -> FLAG.
```c
   char flag = FRAME_FLAG;
   int writtenSize = write(fd, &flag, 1);
   writtenSize += write(fd, frame, frameLength);
   writtenSize += write(fd, &flag, 1);
```
Caso receba REJ, reenvia a mesma trama, caso receba RR, retorna o número de bytes escritos. Caso não receba resposta, o alarme establecido antes da leitura da resposta mostra uma mensagem a cada timeoutSeconds, durante maxTries. Ambos estes argumentos são recebidos como argumentos de chamada do programa, introduzidos pelo utilizador.

### llcloseTransmitter e llcloseReceiver
Em vez do llclose sugerido, foram criados estes dois métodos que executam as partes diferentes do protocolo caso seja transmissor ou recetor.

No llcloseTransmitter, o transmissor envia a trama de supervisão DISC, aguarda por um DISC de resposta e após a receção deste, envia uma trama UA para o recetor e fecha a porta série.
```c
   // Send DISC
   printWarning("Disconecting . . .\n");
   write(fd, FRAME_DISC, FRAME_SUPERVISION_SIZE);

   // Receive DISC
   char bufferAux[FRAME_SUPERVISION_SIZE];
   receiveFrame(fd, bufferAux);

   if (checkIfIsFrame(bufferAux, FRAME_DISC, 0)) {
      write(fd, FRAME_UA, FRAME_SUPERVISION_SIZE);
      close(fd);
      printSuccess("Terminated with Success!\n\n");

      return 0;
   }
```

No llcloseReceiver, o recetor aguarda pela receção de uma trama de supervisão DISC, envia um DISC de resposta e aguarda a receção da trama UA. Após a receção desta, fecha a porta série e termina a execução.
```c
   printWarning("Disconnecting . . .\n");

   int close = 0;
   while(!close) {
      // Receive DISC
      char bufferAux[FRAME_SUPERVISION_SIZE];
      receiveFrame(fd, bufferAux);

      if (checkIfIsFrame(bufferAux, FRAME_DISC, 0)) {
         // Send DISC
         write(fd, FRAME_DISC, FRAME_SUPERVISION_SIZE);

         // Receive UA
         receiveFrame(fd, bufferAux);

         if (checkIfIsFrame(bufferAux, FRAME_UA, 0)) {
            close(fd);
            printSuccess("Terminated with Success!\n\n");
            return 0;
         }
      }
      else {
         continue;
      }
   }
```


## Protocolo de aplicação
> (identificação dos principais aspectos funcionais; descrição da estratégia de implementação destes aspectos com apresentação de extractos de código)

## Validação
> (descrição dos testes efectuados com apresentação quantificada dos resultados, se possível)

## Eficiencia do protocolo de ligação de dados
> (caraterização estatística da  eficiência do protocolo, feita com recurso a medidas sobre o código desenvolvido. A caracterização teórica de um protocolo Stop&Wait, que deverá ser usada como termo de comparação, encontra-se descrita nos slides de Ligação Lógica das aulas teóricas). 

## Conclusões
>(síntese da informação apresentada nas secções anteriores; reflexão sobre os objectivos de aprendizagem alcançados)

## Anexo 1 , um zip com o código fonte