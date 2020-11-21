# RCOM Projeto 1
Autores:  
Filipe Reis Almeida (up201708999)  
Luís Ricardo Matos Mendes (up201604835)

## Sumário
(dois parágrafos: um sobre o contexto do trabalho; outro sobre as principais conclusões do relatório)

Este trabalho aborda a comunicação entre dois computadores via porta série. A realização deste projeto consiste no desenvolvimento de um programa cujo ambiente de execução é um terminal em Linux.  
Conclusões do relatório:


## Introdução
 (indicação dos objectivos do trabalho e do relatório; descrição da lógica do relatório com indicações sobre o tipo de informação que poderá ser encontrada em cada uma secções seguintes)  


## Arquitetura 
(blocos funcionais e interfaces)  
O código está organizado em 5 diretórios diferentes. Os diretórios 'src' e 'include' contém os ficheiros de código fonte .c e os ficheiros de cabeçalho .h, respetivamente.
No diretório 'obj' encontram-se os objetos resultantes da compilação. O diretório 'testFiles' contém os ficheiros utilizados para testar o protocolo e o 'copiedFiles' onde os ficheiros são recebidos depois do envio. O Makefile e os executáveis encontram-se na raíz da pasta.  

O nosso projeto é dividido em duas partes essenciais, a camada de aplicação (App Layer) e a camada de ligação (Link Layer). Ambas partes se encontram nos 


## Estrutura do código
(APIs, principais estruturas de dados, principais funções e sua relação com a arquitetura)

O nosso projeto é dividido em duas partes essenciais, a camada de aplicação (App Layer) e a camada de ligação (Link Layer).  
A camada de aplicação contém as seguintes funções principais:
- appLayerWrite
- appLayerRead

## Casos de uso principais
   (identificação; sequências de chamada de funções)

## Protocolo de ligação lógica
   (identificação dos principais aspectos funcionais; descrição da estratégia de implementação destes aspectos com apresentação de extratos de código)

## Protocolo de aplicacao
   (identificação dos principais aspectos funcionais; descrição da estratégia de implementação destes aspectos com apresentação de extractos de código)

## Validacao
   (descrição dos testes efectuados com apresentação quantificada dos resultados, se possível)

## Eficiencia do protocolo de ligacao de dados
 (caraterização estatística da  eficiência do protocolo, feita com recurso a medidas sobre o código desenvolvido. A caracterização teórica de um protocolo Stop&Wait, que deverá ser usada como termo de comparação, encontra-se descrita nos slides de Ligação Lógica das aulas teóricas). 

## Conclusoes
   (síntese da informação apresentada nas secções anteriores; reflexão sobre os objectivos de aprendizagem alcançados)

## Anexo 1 , um zip com o código fonte