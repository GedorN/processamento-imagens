#include <stdio.h>
#include <stdlib.h>
#include "filtros.h"

int main(int argc, char *argv[] ) {
    int width, height, canais, option = 0;

    system("@cls||clear");

    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("@                                                                   @\n");
    printf("@                                                                   @\n");
    printf("@           BEM VIND0(A) AO EDITOR DE IMAGEM                        @\n");
    printf("@                                                                   @\n");
    printf("@                                                                   @\n");
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

    printf("Insira a largura da janela deslizante: ");
    scanf("%d", &width);

    system("@cls||clear");


    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("@                                                                   @\n");
    printf("@                                                                   @\n");
    printf("@           BEM VIND0(A) AO EDITOR DE IMAGEM                        @\n");
    printf("@                                                                   @\n");
    printf("@                                                                   @\n");
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

    printf("Insira a largura da janela deslizante: %d           Insira a altura da janela deslizante: ", width);
    scanf("%d", & height);

    printf("Número de canais: ");
    scanf("%d", &canais);

    
    putchar('\n');
    putchar('\n');

    while (option != 4) {
        printf("* Agora escolha o tipo do algoritmo a ser aplicado: \n");
        printf("1 - Algoritmo \"ingênuo\"\n");
        printf("2 - Filtro serparável\n");
        printf("3 - Algoritmo com imagens integrais\n");
        printf("4 - Encerrar programa\n");

        printf("Opção: ");

        scanf("%d", &option);

        putchar('\n');
        putchar('\n');

        if (option != 4) {
            processaImagem(argv[1], width, canais, 1, option);
        }

    }
    
    printf("Obrigado por usar o editor de imagem interativo! Esse trabalho vale um 10, não vale?!\n");




    putchar('\n');
    return 0;
}