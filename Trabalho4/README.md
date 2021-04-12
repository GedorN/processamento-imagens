# Trabalho 4

### Modo de uso:
* Insira o comando "make" para compilar o programa
    ```bash
    $ make
    ```
* Execute o programa passando como argumento o caminho das imagens que contém arroz. Exemplo:
    ```bash
    $ ./exe 60.bmp
    ```


### Sobre o trabalho

 Para tentar estimar o mais próximo possível a quantidade de arroz nas imagens, foi usado duas tratativas: Manipular a imagem retirando o fundo, removendo ruído e binarizando. A partir disso foi usado a abordagem de "flood fill" para obtenção dos componentes conexos; O segundo passo foi tratar os componentes encontrados com fórmulas estatísticas para aproximar quantidades de arroz agrupados.

 Para tratamento da imagem foram usadados os seguintes passos:

* 2x filtro da média para remoção do fundo
* Binarização da imagem
* Utilização de erosão da imagem para remoção de ruído

Terminado esse tratamento, o número de componentes é contado:
* Flood Fill para contar componentes conexos
* Cáculo da média dos componentes, com exclusão do outliers (componentes grandes demais)
* Componentes com tamanho até a média são considerados como 1 unidade; componentes maiores que a média são dividos pela média (com exlcusão do desvio padrão) para tentar estimar quantos arrozes estão contidos no blob

