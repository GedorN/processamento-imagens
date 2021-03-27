#include <stdio.h>
#include "imagem.h"
#include "filtros2d.h"
#include "base.h"
Imagem*  integral (Imagem  *img, Imagem *img_out, int window_height, int window_width) {
    Imagem *img_buffer;
    img_buffer = criaImagem(img->largura, img->altura, img->n_canais);

    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = 0; i < img->altura; i++) {
            if (i != 0) {
                img_buffer->dados[canal][i][0] = img->dados[canal][i][0] + img_buffer->dados[canal][i - 1][0];
            } else {
                img_buffer->dados[canal][0][0] = img->dados[canal][0][0];
                for (int j = 1; j < img->largura; j++) {
                    img_buffer->dados[canal][i][j] = img->dados[canal][i][j] + img_buffer->dados[canal][i][j - 1];
                }
            }
            
        }
    }

    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = 1; i < img->altura; i++) {
            for (int j = 1; j < img->largura; j++) {
                img_buffer->dados[canal][i][j] =  img->dados[canal][i][j] + img_buffer->dados[canal][i][j - 1]+ img_buffer->dados[canal][i - 1][j] - img_buffer->dados[canal][i - 1][j - 1];
            }
        }
    } // integral da imagem calculada

    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = 0; i < img->altura; i++) {
            for (int j = 0; j < img->largura; j++) {
                int i_window_top = (i - window_height / 2) >= 0 ? (i - window_height / 2) : 0;
                int i_window_bot = (i + window_height / 2) < img->altura ? (i +  window_height / 2) : (img->altura - 1);
                int j_window_left = (j - window_width / 2) >= 0 ? (j - window_width / 2) : 0;
                int j_window_right = (j + window_width /2) < img->largura ? (j + window_width /2) : (img->largura -1);

                float D = img_buffer->dados[canal][i_window_bot][j_window_right];
                float A = (i_window_top - 1 >= 0) && (j_window_left - 1 >= 0) ? img_buffer->dados[canal][i_window_top - 1 ][j_window_left - 1] : 0;
                float C = (j_window_left - 1 >= 0) ? img_buffer->dados[canal][i_window_bot][j_window_left - 1] : 0;
                float B = (i_window_top - 1 >= 0) ? img_buffer->dados[canal][i_window_top - 1][j_window_right] : 0;

                float area  = D + A - C - B;
                int size = (i_window_bot - i_window_top + 1) * (j_window_right - j_window_left + 1);
                img_out->dados[canal][i][j] = area/size;
            }
        }
    }

    destroiImagem(img_buffer);
    return img_out;
}

int main () {
    printf("Olá\n");
    int suavizacao = 3; //7
    Imagem *img = abreImagem("WindWakerGC.bmp", 3);
    Imagem *light_mask = criaImagem(img->largura, img->altura, img->n_canais);
    Imagem *img_out = criaImagem(img->largura, img->altura, img->n_canais);
    Imagem *aux = criaImagem(img->largura, img->altura, img->n_canais);
    Imagem *buffer = criaImagem(img->largura, img->altura, img->n_canais);
    Imagem *buffer_aux = criaImagem(img->largura, img->altura, img->n_canais);
    
    for (int i = 0; i < img->altura; i++) {
        for (int j = 0; j < img->largura; j++) {
            double light = (img->dados[0][i][j] + img->dados[1][i][j]+ img->dados[2][i][j] )/ 3;
            if (light > 0.52) {
                light_mask->dados[0][i][j] = img->dados[0][i][j];
                light_mask->dados[1][i][j] = img->dados[1][i][j];
                light_mask->dados[2][i][j] = img->dados[2][i][j];
            } else {
                light_mask->dados[0][i][j] = 0;
                light_mask->dados[1][i][j] = 0;
                light_mask->dados[2][i][j] = 0;
            }
        }
    }
    aux = clonaImagem(light_mask);
    buffer_aux=  integral(light_mask, buffer_aux, 20, 20);

    for (int i = 0; i < 2 * suavizacao; i++) {
        buffer_aux=  integral(aux, buffer_aux, 10, 10);
        // aux = clonaImagem(buffer_aux);
        soma(buffer, buffer_aux, 1, 1, buffer);
    }

    // for (int i = 0; i < suavizacao; i++) {
    //     filtroGaussiano(aux, buffer_aux, (i + 1) * 10, (i + 1) * 10, NULL);
    //     soma(buffer, buffer_aux, 1, 1, buffer);
    // }

    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = 0; i < img->altura; i++) {
            for (int j = 0; j < img->largura; j++) {
                double sum = 0.08 * buffer->dados[canal][i][j] + 1 * img->dados[canal][i][j];
                img_out->dados[canal][i][j] = sum > 1 ? 1 : sum; 
            }
        }
    }




    salvaImagem(img, "teste.bmp");
    salvaImagem(light_mask, "mask1.bmp");
    salvaImagem(buffer_aux, "mask2.bmp");
    salvaImagem(buffer, "mask3.bmp");
    salvaImagem(img_out, "img_out.bmp");
    destroiImagem((light_mask));
    destroiImagem(img);
    destroiImagem(img_out);
    destroiImagem(buffer);
    destroiImagem(aux);
    destroiImagem(buffer_aux);
    return 0;
}

