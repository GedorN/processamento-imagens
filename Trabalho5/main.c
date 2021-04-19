#include <stdio.h>
#include "imagem.h"
#include "segmenta.h"
#include "filtros2d.h"
#include "cores.h"


Imagem* integral (Imagem  *img, Imagem *img_buffer) {

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

    return img_buffer;
}

Imagem* filter_with_integral(Imagem *img_out, Imagem* img_integral, int window_height, int window_width) {
    for (int canal = 0; canal < img_integral->n_canais; canal++) {
        for (int i = 0; i < img_integral->altura; i++) {
            for (int j = 0; j < img_integral->largura; j++) {
                int i_window_top = (i - window_height / 2) >= 0 ? (i - window_height / 2) : 0;
                int i_window_bot = (i + window_height / 2) < img_integral->altura ? (i +  window_height / 2) : (img_integral->altura - 1);
                int j_window_left = (j - window_width / 2) >= 0 ? (j - window_width / 2) : 0;
                int j_window_right = (j + window_width /2) < img_integral->largura ? (j + window_width /2) : (img_integral->largura -1);

                float D = img_integral->dados[canal][i_window_bot][j_window_right];
                float A = (i_window_top - 1 >= 0) && (j_window_left - 1 >= 0) ? img_integral->dados[canal][i_window_top - 1 ][j_window_left - 1] : 0;
                float C = (j_window_left - 1 >= 0) ? img_integral->dados[canal][i_window_bot][j_window_left - 1] : 0;
                float B = (i_window_top - 1 >= 0) ? img_integral->dados[canal][i_window_top - 1][j_window_right] : 0;

                float area  = D + A - C - B;
                int size = (i_window_bot - i_window_top + 1) * (j_window_right - j_window_left + 1);
                img_out->dados[canal][i][j] = area/size;
            }
        }
    }

    return img_out;
}
int main () {
    Imagem* img = abreImagem("./img/7.bmp", 3);
    Imagem *img_galssian = criaImagem(img->largura, img->altura, 3);
    Imagem *aux = criaImagem(img->largura, img->altura, 3);
    Imagem *out = criaImagem(img->largura, img->altura, 1);
    integral(img, aux );
    filter_with_integral(img_galssian, aux, 17, 17);
    // printf("Calculando...\n");
    // printf("Valor do Otsu: %f\n", otsu);
    // printf("%f\n", img->dados[0][0][0]);
    // printf("%f\n", img->dados[1][0][0]);
    // printf("%f\n", img->dados[2][0][0]);
    for (int i = 0; i < img->altura; i++) {
        for (int j = 0; j < img->largura; j++) {
            out->dados[0][i][j] = img_galssian->dados[1][i][j];
            // if (out->dados[0][i][j] >= otsu) {
            //     out->dados[0][i][j] = 0;
            // } else {
            //     out->dados[0][i][j] = 1;
            // }
        }
    }
    float otsu = thresholdOtsu(out);

    for (int i = 0; i < img->altura; i++) {
        for (int j = 0; j < img->largura; j++) {
            if (out->dados[0][i][j] >= otsu) {
                out->dados[0][i][j] = 0;
            } else {
                out->dados[0][i][j] = 1;
            }
        }
    }

    salvaImagem(out, "out.bmp");
    destroiImagem(img);
    destroiImagem(img_galssian);
    destroiImagem(aux);
    destroiImagem(out);

    return 0;
}