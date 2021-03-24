#include  <stdio.h>
#include <math.h>
#include "imagem.h"
#include "filtros.h"

int  integral (Imagem  *img, int window_height, int window_width) {
    Imagem *img_out, *img_buffer;
    img_out = criaImagem(img->largura, img->altura, img->n_canais);
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

    salvaImagem(img_out, "img-integral-out.bmp");
    destroiImagem(img_out);
    destroiImagem(img_buffer);
    return 1;
}

int ingenuo (Imagem *img, int window_height, int window_width) {
    Imagem *img_out;
    img_out = criaImagem(img->largura, img->altura, img->n_canais);

    // pintando o fundo do imagem
    for (int canal = 0; canal < img_out->n_canais; canal++) {
        for (int i = 0; i < img_out->altura; i++) {
            for (int j = 0; j < img_out->largura; j++) {
                img_out->dados[canal][i][j] = 0;
            }
        }
    }

    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = window_height /2 ; i < img->altura - window_height /2; i++) {
            for (int j = window_width /2; j < img->largura - window_width/2; j++) {


                double sum = 0;
                int pixels = 0;
                for (int i_window = (i - window_height / 2); i_window <= i +window_height /2; i_window++) {
                    for (int j_window = (j - window_width / 2); j_window <= j + (window_width / 2); j_window++) {
                        sum += img->dados[canal][i_window][j_window];
                        pixels++;
                    }
                }
                img_out->dados[canal][i][j] = sum / pixels;

            }
        }
    }

    salvaImagem(img_out, "img-ingenuo-out.bmp");
    destroiImagem(img_out);
    return 1;
}

int separavel(Imagem *img, int window_height, int window_width) {
    Imagem *img_out, *img_buffer;
    img_out = criaImagem(img->largura, img->altura, img->n_canais);
    img_buffer = criaImagem(img->largura, img->altura, img->n_canais);

    // pintando o fundo do imagem
    for (int canal = 0; canal < img_out->n_canais; canal++) {
        for (int i = 0; i < img_out->altura; i++) {
            for (int j = 0; j < img_out->largura; j++) {
                img_out->dados[canal][i][j] = 0;
            }
        }
    }


    double sum = 0;
    for (int canal = 0; canal < img->n_canais; canal ++) {
        for (int i = window_height / 2; i < img->altura - window_height /2; i++) {
            for (int j = window_width / 2; j < img->largura - window_width /2; j++) {
                if (j == window_width / 2) {
                    sum = 0;
                    sum+= img->dados[canal][i][j];
                    for (int pixel = 1; pixel <= window_width /2; pixel++) {
                        sum+= img->dados[canal][i][j + pixel];
                        sum+= img->dados[canal][i][j - pixel];
                    }
                    sum = sum /window_width;
                    img_buffer->dados[canal][i][j] = sum;
                } else {
                    sum -= img->dados[canal][i][(j - window_width / 2) - 1] / window_width;
                    sum += img->dados[canal][i][(j + window_width / 2)] / window_width;
                    img_buffer->dados[canal][i][j] = sum;
                }
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
                    sum -= img_buffer->dados[canal][(i - window_height / 2) - 1][j] / window_height;
                    sum += img_buffer->dados[canal][(i + window_height / 2)][j] / window_height;
                    img_out->dados[canal][i][j] = sum;
                }
            }
        }
    }

    salvaImagem(img_out, "img-separavel-out.bmp");
    destroiImagem(img_out);
    destroiImagem(img_buffer);

    return 1;
}


int processaImagem(char* imagem, int window_width, int window_height, int canais, int opcao) {
        Imagem *img = abreImagem(imagem, canais);
        int result = 0;

        printf("Processando a imagem...\n");
        switch (opcao) {
        case 1:
            result = ingenuo(img, window_height, window_width);
            break;
        case 2:
            result = separavel(img, window_height, window_width);
            break;
        case 3:
            result = integral(img, window_height, window_width);
            break;
        default:
            break;
        }

        if (result == 1) {
            printf("Imagem processada com sucesso!\n");
        } else {
            printf("Erro desconhecido ao processar a imagem\n");
        }


        destroiImagem(img);
        return result;
    
}