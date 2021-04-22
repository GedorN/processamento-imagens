#include <stdio.h>
#include "imagem.h"
#include "segmenta.h"
#include "filtros2d.h"
#include "cores.h"

int main () {
    Imagem* img = abreImagem("./img/5.bmp", 3);
    // Imagem *img_galssian = criaImagem(img->largura, img->altura, 1);
    Imagem *img_galssian = clonaImagem(img);

    Imagem *aux = criaImagem(img->largura, img->altura, 3);
    Imagem *out = criaImagem(img->largura, img->altura, 1);

    Imagem *r = criaImagem(img->largura, img->altura, 1);
    Imagem *g = criaImagem(img->largura, img->altura, 1);
    Imagem *b = criaImagem(img->largura, img->altura, 1);

    RGBParaHSL(img, aux);

    for (int i = 0; i < aux->altura; i++) {
        for (int j = 0; j < aux->largura; j++) {
            if (i == 0 && j ==0) {
                printf("%f\n", aux->dados[0][0][0]);
                printf("%f\n", aux->dados[1][0][0]);
                printf("%f\n", aux->dados[2][0][0]);
            }
            float hue = aux->dados[0][i][j];
            float saturation = aux->dados[1][i][j];
            float luminance = aux->dados[2][i][j];
            if (hue > 60 && hue < 180 && saturation >= 0.170 && luminance >= 0.120) {
                out->dados[0][i][j] = 0;
            } else {
                out->dados[0][i][j] = 1;
            }
        }
    }

    aux = clonaImagem(out);


    //  =========================================================
    // Usando dilatação para remover ruídos das máscaras. Pode não ser a melhor a abordagem
    Imagem *kernel = criaImagem(10 ,10 , 1);
    Coordenada cod;
    cod.x = 1;
    cod.y= 1;
    for (int canal = 0; canal < 1; canal++) {
        for (int i = 0; i < kernel->altura; i++) {
            for (int j = 0; j < kernel->largura; j++) {
                kernel->dados[canal][i][j] = 1;
            }
        }
    }

    kernel->dados[0][cod.x][cod.y] = 0;

    dilata(aux, kernel, cod, out);
    // ====================================================================================================

    salvaImagem(out, "out.bmp");

    salvaImagem(r, "r.bmp");
    salvaImagem(g, "g.bmp");
    salvaImagem(b, "b.bmp");

    destroiImagem(img);
    destroiImagem(img_galssian);
    destroiImagem(aux);
    destroiImagem(out);
    destroiImagem(kernel);
    destroiImagem(b);
    destroiImagem(g);
    destroiImagem(r);

    return 0;
}