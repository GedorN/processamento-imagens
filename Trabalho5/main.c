#include <stdio.h>
#include "imagem.h"
#include "segmenta.h"
#include "filtros2d.h"
#include "cores.h"

#define KERNEL_I 2
#define KERNEL_J 6

borderProcessing(Imagem* in, Imagem* background, Imagem* mask, Imagem* out, int **kernel) {
    Imagem* aux = clonaImagem(in);
    filtroGaussiano(in, aux, 15, 15, NULL);
    for (int i = 0; i < in->altura; i++) {
        for (int j = 0; j < in->largura; j++) {

            int apply_kernel = 1;

            if ((j + KERNEL_J - 1) < mask->largura && (i + KERNEL_I - 1) < mask->altura) {

                for (int ki = 0; ki < KERNEL_I; ki++) {
                    for (int kj = 0; kj < KERNEL_J; kj++) {
                        if (kernel[ki][kj] == 1 && mask->dados[0][i + ki][j + kj] == 0) {
                            apply_kernel = 0;
                            break;
                        }
                        if (kernel[ki][kj] == 0 && mask->dados[0][i + ki][j + kj] == 1) {
                            apply_kernel = 0;
                            break;
                        }
                    }
                    if (apply_kernel == 0) {
                        break;
                    }
                }

                if (apply_kernel == 1) {
                    for (int ki = 0; ki < KERNEL_I; ki++) {
                        for (int kj = 0; kj < KERNEL_J; kj++) {
                            if (kernel[ki][kj] == 1) {
                                out->dados[0][i + ki][j + kj] = in->dados[0][i + ki][j + kj];
                                out->dados[1][i + ki][j + kj] = in->dados[1][i + ki][j + kj];
                                out->dados[2][i + ki][j + kj] = in->dados[2][i + ki][j + kj];
                            } else {
                                out->dados[0][i + ki][j + kj] = aux->dados[0][i + ki][j + kj];
                                out->dados[1][i + ki][j + kj] = aux->dados[1][i + ki][j + kj];
                                out->dados[2][i + ki][j + kj] = aux->dados[2][i + ki][j + kj];
                            }
                        }
                    }                    
                }


            }
        }
    }
    destroiImagem(aux);
}

int main () {
    Imagem* img = abreImagem("./img/0.bmp", 3);
    Imagem *aux = criaImagem(img->largura, img->altura, 3);

    Imagem *background = criaImagem(img->largura, img->altura, 3);
    Imagem *background_aux = abreImagem("./background.bmp", 3);
    redimensionaNN(background_aux, background);
    destroiImagem(background_aux);


    
    Imagem *mask = criaImagem(img->largura, img->altura, 1);
    Imagem *out = criaImagem(img->largura, img->altura, 3);



    RGBParaHSL(img, aux);


    // Cria a máscara binária
    for (int i = 0; i < aux->altura; i++) {
        for (int j = 0; j < aux->largura; j++) {
            float hue = aux->dados[0][i][j];
            float saturation = aux->dados[1][i][j];
            float luminance = aux->dados[2][i][j];
            if ((hue > 80 && hue < 160) && (saturation >= 0.128 && saturation >= 0.320) && (luminance >= 0.120 && luminance <= 0.8)) {
                mask->dados[0][i][j] = 0;
            } else {
                mask->dados[0][i][j] = 1;
            }
        }
    }

    filtroMedianaBinario(mask, mask, 3, 3, NULL);



    //  Passa a imagem de entrada e o background pela máscara
    for (int i = 0; i < background->altura; i++) {
        for (int j = 0; j < background->largura; j++) {
            if (mask->dados[0][i][j] == 1) {
                out->dados[0][i][j] = img->dados[0][i][j];
                out->dados[1][i][j] = img->dados[1][i][j];
                out->dados[2][i][j] = img->dados[2][i][j];
            } else {
                out->dados[0][i][j] = background->dados[0][i][j];
                out->dados[1][i][j] = background->dados[1][i][j];
                out->dados[2][i][j] = background->dados[2][i][j];
            }

        }
    }


    // Tratamento de bordas
    int kernel[KERNEL_I][KERNEL_J] = { {0, 0, 0, 0, 1, 1},
                                       {0, 0, 0, 0, 1, 1}},
    
    borderProcessing(img, background, mask, out, kernel);


    salvaImagem(mask, "mask.bmp");
    salvaImagem(out, "out.bmp");

    destroiImagem(img);
    destroiImagem(aux);
    destroiImagem(mask);
    destroiImagem(out);
    destroiImagem(background);

    return 0;
}