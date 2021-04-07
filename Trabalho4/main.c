#include <stdio.h>
#include <stdlib.h>
#include "imagem.h"
#include "filtros2d.h"
#include "segmenta.h"

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
    Imagem *img = abreImagem("82.bmp", 3);
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
    out = filter_with_integral(out, img_integral, 295, 295);
    salvaImagem(out, "0.bmp");
    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = 0; i < img->altura; i++) {
            for (int j = 0; j < img->largura; j++) {
                out->dados[canal][i][j] = (img->dados[canal][i][j] * 1) - (out->dados[canal][i][j] * 1);
            }
        }
    }
    salvaImagem(out, "1.bmp");
    Imagem *aux= criaImagem(out->largura, out->altura, out->n_canais);
    for (int i = 0; i < out->altura; i++) {
        for (int j = 0; j < out->largura; j++) {
            double value = out->n_canais > 1 ? (out->dados[0][i][j] * 0.299f) + (out->dados[1][i][j] * 0.587f)+ (out->dados[2][i][j] * 0.114f) : out->dados[0][i][j];
            if (value >= 0.25) {
                if (out->n_canais > 1) {
                    aux->dados[0][i][j] = 1;
                    aux->dados[1][i][j] = 1;
                    aux->dados[2][i][j] = 1;
                } else {
                    aux->dados[0][i][j] = 1;
                }
                
            } else {
                if (out->n_canais > 1) {
                    aux->dados[0][i][j] = 0;
                    aux->dados[1][i][j] = 0;
                    aux->dados[2][i][j] = 0;
                } else {
                    aux->dados[0][i][j] = 0;
                }
            }
        }
    }
    salvaImagem(aux, "2.bmp");

    // ===================
    Imagem *final = criaImagem(out->largura, out->altura, out->n_canais);
    Imagem *kernel = criaImagem(3, 3, 1);
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

    erode(aux, kernel, cod, final);
   

    salvaImagem(final, "3.bmp");
    ComponenteConexo *componente;
    printf("Vou chamar: \n");

    int components = rotulaFloodFill(final, &componente, 2, 2, 2);
    printf("No final a verdade foi: %d\n", components);

    float a = componente[0].label;
    printf("%f", a);
    putchar('\n');

    printf("superou\n");

    for (int  i = 0; i < components; i++) {
        printf("%d\n", componente[i].n_pixels);
    }

    free(componente);

    destroiImagem(img);
    destroiImagem(img_integral);
    destroiImagem(out);
    destroiImagem(kernel);
    destroiImagem(final);


    return 0;
}