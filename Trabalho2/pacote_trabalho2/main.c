#include  <stdio.h>
#include "imagem.h"

int main() {
    Imagem *img;
    img = abreImagem("./Exemplos/a01 - Original.bmp", 1);
    salvaImagem(img, "somebody.bmp");

    destroiImagem(img);
}