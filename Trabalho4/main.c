#include <stdio.h>
#include "imagem.h"
#include "filtros2d.h"
Imagem* threshold_with_integral(Imagem *img, Imagem *img_out, Imagem* img_integral, int window_height, int window_width) {
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
                // printf("Average: %f - %f\n", area, img_integral->dados[canal][i][j]);
                area = area - img->dados[canal][i][j];
                int size = (i_window_bot - i_window_top + 1) * (j_window_right - j_window_left + 1) - 1;
                float average = area / size;
                img_out->dados[canal][i][j] = img->dados[canal][i][j] > average ?  1 : 0;  
            }
        }
    }

    return img_out;
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

int main() {
    Imagem *img = abreImagem("150.bmp", 3);
    Imagem *img_integral = criaImagem(img->largura, img->altura, img->n_canais);
    Imagem *out = clonaImagem(img);

    // 
    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = 0; i < img->altura; i++) {
            for (int j = 0; j < img->largura; j++) {
                img_integral->dados[canal][i][j] = 0;
            }
        }
    }

    // UNO

    img_integral = integral(img, img_integral);
    out = filter_with_integral(out, img_integral, 125, 125);
    salvaImagem(out, "0.bmp");
    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = 0; i < img->altura; i++) {
            for (int j = 0; j < img->largura; j++) {
                out->dados[canal][i][j] = img->dados[canal][i][j] - out->dados[canal][i][j];
            }
        }
    }
    salvaImagem(out, "1.bmp");

    // ===================
    Imagem *aux = clonaImagem(out);

    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = 0; i < img->altura; i++) {
            for (int j = 0; j < img->largura; j++) {
                img_integral->dados[canal][i][j] = 0;
            }
        }
    }
    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = 0; i < img->altura; i++) {
            for (int j = 0; j < img->largura; j++) {
                out->dados[canal][i][j] = 0;
            }
        }
    }
    img_integral = integral(aux, img_integral);
    out = filter_with_integral(out, img_integral, 75, 75);
    salvaImagem(out, "2.bmp");
     for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = 0; i < img->altura; i++) {
            for (int j = 0; j < img->largura; j++) {
                out->dados[canal][i][j] = aux->dados[canal][i][j] - out->dados[canal][i][j];
            }
        }
    }


    salvaImagem(out, "3.bmp");
    destroiImagem(img);
    destroiImagem(img_integral);
    destroiImagem(out);
    destroiImagem(aux);


    return 0;
}