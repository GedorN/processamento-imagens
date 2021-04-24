# Trabalho 5

### Modo de uso:
* Insira o comando "make" para compilar o programa
    ```bash
    $ make
    ```
* Execute o programa passando como primeiro argumento o caminho da imagem com chroma key e segundo o background a ser inserido no chroma key. Exemplo:
    ```bash
    $ ./exe image.bmp background.bmp
    ```

### Sobre o trabalho:

Para trabalhar com o chroma key, o primeiro passo foi converter a imagem de RGB para HSL, onde temos uma maior facilidade de trabalhar com o que é verde. Feito isso, foram traçados valores para definir o intervalo de matiz, saturação e luminância do verde do chroma key para gerar uma máscara binária do que é fundo e do que não é. Por último, foi feito um tratamento nas bordas para que as transições de fundo para imagem da frente não fossem tão bruscas

