# Trabalho 3

   Para compilar, insira digite o comando make:
  ```bash
  $ make
  ```
  Será criado dois arquivos executáveis no diretório. O "gaussian" e o "boxblur".
  
  * gaussian: Executável que fará o bloom lighting com filtro gaussiano
  * boxblur: Executável que fará o bloom lighting com o filtro da média com janela deslizante

### Rodando os programas:
  Os executáveis esperam como parâmetro o nome da imagem a ser tratada. Exemplo:
  ```bash
  $ ./gaussian minha-imagem.bmp
  ```

  o resultado será uma imagem de saída com o bloom light aplicado.

  Obs: Caso tenha problema no seu computador usando o GCC como compilador, basta abir o arquivo makefile e mudar a variável CP para o seu compilador de C.
