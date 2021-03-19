#include  <stdio.h>
#include <math.h>
#include "imagem.h"

int main() {
    Imagem *img, *img_out, *img_buffer;
    int window_size = 8;
    int window_width = 1;
    int window_height = 11;
    img = abreImagem("./Exemplos/a01 - Original.bmp", 1);
    img_out = criaImagem(img->largura, img->altura, img->n_canais);
    printf("\n[Dimensões] %d x %d \n", img->altura, img->largura);
    img_buffer = criaImagem(img->largura, img->altura, img->n_canais);
    printf("\n[Dimensões] %d x %d \n", img_out->altura, img_out->largura);
    printf("\n[Dimensões] %d x %d \n", img_buffer->altura, img_buffer->largura);
    printf("canal: %d \n", img->n_canais);
    for (int canal = 0; canal < img_out->n_canais; canal++) {
        for (int i = 0; i < img_out->altura; i++) {
            for (int j = 0; j < img_out->largura; j++) {
                img_out->dados[canal][i][j] = 0;

            }
        }
    }

    // Filtro da média separável
    
    /*double sum = 0;
    for (int canal = 0; canal < img->n_canais; canal ++) {
        for (int i = window_height / 2; i < img->altura - window_height /2; i++) {
            for (int j = window_width / 2; j < img->largura - window_width /2; j++) {
                if (j == window_width / 2) {
                    sum = 0;
                    sum+= img->dados[canal][i][j];
                    for (int pixel = 1; pixel <= window_width /2; pixel++) {
                        // printf("Entrou %d\n", j);
                        sum+= img->dados[canal][i][j + pixel];
                        sum+= img->dados[canal][i][j - pixel];
                        // printf("Janela: [%d, %d]\n", j - pixel, j + pixel);
                    }
                    sum = sum /window_width;
                    img_buffer->dados[canal][i][j] = sum;
                } else {
                    sum -= img->dados[canal][i][(j - window_width / 2) - 1] / window_width;
                    sum += img->dados[canal][i][(j + window_width / 2)] / window_width;
                    // printf("Janela: [%d, %d]\n", (j - window_width / 2), (j + window_width / 2) + 1);
                    img_buffer->dados[canal][i][j] = sum;
                }
                // printf("Janela: [%d, %d, %d]\n", canal,i, j);
                // for (int pixel = -(window_width/2); pixel <= window_width/2; pixel++) {
                //     sum += img->dados[canal][i][j - pixel];
                //     sum += img->dados[canal][i][j + pixel];
                // }

                // img_buffer->dados[canal][i][j] = sum/((2 * window_width) + 1);
                // img_out->dados[canal][i][j] = sum / ((2* window_height) + 1);

            }
        }
    }
   for (int canal = 0; canal < img_buffer->n_canais; canal ++) {
        for (int j = window_width / 2; j < img_buffer->largura - window_width /2; j++) {
            for (int i = window_height / 2; i < img_buffer->altura - window_height /2; i++) {
               if (i == window_height / 2) {
                    sum = 0;
                    sum+= img_buffer->dados[canal][i][j];
                    for (int pixel = 1; pixel <= window_height /2; pixel++) {
                        sum+= img_buffer->dados[canal][i + pixel][j];
                        sum+= img_buffer->dados[canal][i  - pixel][j];
                    }
                    sum = sum/window_height;
                    img_out->dados[canal][i][j] = sum;
                }
                else {
                    printf("Janela: [%d, %d]\n", ((i - window_height / 2) - 1), (i + window_height / 2) );
                    sum -= img_buffer->dados[canal][(i - window_height / 2) - 1][j] / window_height;
                    sum += img_buffer->dados[canal][(i + window_height / 2)][j] / window_height;
                    img_out->dados[canal][i][j] = sum;
                }

                // printf("SUm: %lf\n", sum);
            }
        }
    }*/



    // Algoritmo ingênuo
    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = window_height /2 ; i < img->altura - window_height /2; i++) {
            for (int j = window_width /2; j < img->largura - window_width/2; j++) {


                double sum = 0;
                int pixels = 0;
                printf("Centro: %d %d\n", i, j);
                printf("\n{IW: [%d], [%d]  JW: [%d][%d]}\n", (i - window_height / 2), i +window_height /2, (j - window_width / 2), j + (window_width / 2));
                for (int i_window = (i - window_height / 2); i_window <= i +window_height /2; i_window++) {
                    for (int j_window = (j - window_width / 2); j_window <= j + (window_width / 2); j_window++) {
                        printf("IW: %d  JW: %d\n", i_window, j_window);
                        sum += img->dados[canal][i_window][j_window];
                        pixels++;
                    }
                }
                printf("Sum: %lf \n", sum);
                printf("pixels: %d \n ", pixels);
                printf("Sum per pixel: %lf\n", sum/pixels);
                img_out->dados[canal][i][j] = sum / pixels;

            }
        }
    }
    printf("\n[Dimensões index] %d x %d \n", img_out->altura -1, img_out->largura-1);
    printf("\n[Dimensões] %d x %d \n", img_buffer->altura, img_buffer->largura);
    printf("canal: %d \n", img->n_canais);
    salvaImagem(img, "somebody.bmp");
    // salvaImagem(img_buffer, "somebody-buffer.bmp");
    salvaImagem(img_out, "somebody-out.bmp");
    destroiImagem(img);
    destroiImagem(img_out);
    destroiImagem(img_buffer);

}