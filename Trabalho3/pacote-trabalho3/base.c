/*============================================================================*/
/* TIPOS E FUN��ES B�SICOS                                                    */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu - nassu@dainf.ct.utfpr.edu.br                       */
/*============================================================================*/
/** Tipos e fun��es b�sicos, �teis para diversos problemas envolvendo
 * manipula��o de imagens. */
/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include "filtros2d.h"

/*----------------------------------------------------------------------------*/
/** Soma ponderada de imagens, sem qualquer tratamento adicional.
 *
 * Par�metros: Imagem* in1: imagem de entrada. Se tiver mais que 1 canal,
 *               processa cada canal independentemente.
 *             Imagem* in2: segunda imagem entrada, do mesmo tamanho que in1.
 *             float mul1: multiplicador para a primeira imagem.
 *             float mul2: multiplicador para a segunda imagem.
 *             Imagem* out: imagem de sa�da. Deve ter o mesmo tamanho da
 *               imagem de entrada.
 *
 * Valor de retorno: nenhum. */

void soma (Imagem* in1, Imagem* in2, float mul1, float mul2, Imagem* out)
{
    if (in1->largura != out->largura || in1->altura != out->altura || in1->n_canais != out->n_canais ||
        in2->largura != out->largura || in2->altura != out->altura || in2->n_canais != out->n_canais)
    {
        printf ("ERRO: soma: as imagens precisam ter o mesmo tamanho e numero de canais.\n");
        exit (1);
    }

    int channel, row, col;
    for (channel = 0; channel < in1->n_canais; channel++)
        for (row = 0; row < in1->altura; row++)
            for (col = 0; col < in1->largura; col++)
                out->dados [channel][row][col] = in1->dados [channel][row][col]*mul1 + in2->dados [channel][row][col]*mul2;
}

/*============================================================================*/
/* NORMALIZA��O                                                               */
/*============================================================================*/
/** Normaliza��o global, remapeia os pixels de uma imagem para que se ajustem
 * a uma faixa dada. Os canais da imagem s�o normalizados independentemente.
 *
 * Par�metros: Imagem* in: imagem de entrada.
 *             Imagem* out: imagem de sa�da. Deve ter o mesmo tamanho da
 *               imagem de entrada.
 *             float min: valor inferior da faixa desejada.
 *             float max: valor superior da faixa desejada.
 *
 * Valor de retorno: nenhum (usa a imagem de sa�da). */

void normaliza (Imagem* in, Imagem* out, float min, float max)
{
    if (in->largura != out->largura || in->altura != out->altura || in->n_canais != out->n_canais)
    {
        printf ("ERRO: normaliza: as imagens precisam ter o mesmo tamanho e numero de canais.\n");
        exit (1);
    }

    if (max <= min)
    {
        printf ("ERRO: normaliza: max deve ser maior que min.\n");
        exit (1);
    }

    int channel, row, col;
    float min_in, max_in, intervalo_in, intervalo_out = max - min;

    // Normaliza os canais da imagem de forma independente.
    for (channel = 0; channel < in->n_canais; channel++)
    {
        // Encontra o intervalo inicial.
        min_in = in->dados [channel][0][0];
        max_in = min_in;

        for (row = 0; row < in->altura; row++)
            for (col = 0; col < in->largura; col++)
            {
                float val = in->dados [channel][row][col];
                if (val < min_in)
                    min_in = val;
                if (val > max_in)
                    max_in = val;
            }

        intervalo_in = max_in - min_in;

        if (intervalo_in < 0.0001f || intervalo_in == intervalo_out)
        {
            for (row = 0; row < in->altura; row++)
                for (col = 0; col < in->largura; col++)
                    out->dados [channel][row][col] = in->dados [channel][row][col]; // Imagem homog�nea ou j� normalizada. Fica como est�.
        }
        else
        {
            for (row = 0; row < in->altura; row++)
                for (col = 0; col < in->largura; col++)
                    out->dados [channel][row][col] = (in->dados [channel][row][col] - min_in) / intervalo_in * intervalo_out + min; // Normaliza.
        }
    }
}

/*----------------------------------------------------------------------------*/
/** Normaliza��o local "ing�nua". Toma os m�ximos e m�nimos locais ao redor de
 * cada pixel, e normaliza cada pixel com base nesses intervalos. Inclui um
 * passo de suaviza��o para evitar a cria��o de artefatos quando surge um valor
 * extremo.
 *
 * Par�metros: Imagem* in: imagem de entrada. Se tiver mais que 1 canal,
 *               processa cada canal independentemente.
 *             Imagem* out: imagem de sa�da. Deve ter o mesmo tamanho da
 *               imagem de entrada.
 *             float min: valor inferior da faixa desejada.
 *             float max: valor superior da faixa desejada.
 *             int largura: largura/altura da janela. Deve ser �mpar.
 *
 * Valor de retorno: nenhum. */

void normLocalSimples (Imagem* in, Imagem* out, float min, float max, int largura)
{
    if (in->largura != out->largura || in->altura != out->altura || in->n_canais != out->n_canais)
    {
        printf ("ERRO: normLocalSimples: as imagens precisam ter o mesmo tamanho e numero de canais.\n");
        exit (1);
    }

    if (largura % 2 == 0)
    {
        printf ("ERRO: normLocalSimples: a janela deve ter largura impar.\n");
        exit (1);
    }

    if (max <= min)
    {
        printf ("ERRO: normLocalSimples: max deve ser maior que min.\n");
        exit (1);
    }

    Imagem* buffer = criaImagem (in->largura, in->altura, in->n_canais);
    Imagem* img_max = criaImagem (in->largura, in->altura, in->n_canais);
    Imagem* img_min = criaImagem (in->largura, in->altura, in->n_canais);
    Imagem* img_aux = criaImagem (in->largura, in->altura, in->n_canais);

    // Come�a encontrando os m�ximos e m�nimos locais. Coloca em uma imagem auxiliar e "borra".
    int channel, row, col;
    maxLocal (in, img_aux, largura, largura, buffer);
    blur (img_aux, img_max, largura, largura, buffer);
    minLocal (in, img_aux, largura, largura, buffer);
    blur (img_aux, img_min, largura, largura, buffer);

    // Agora normaliza cada pixel com base na sua vizinhan�a.
    float intervalo_in, region_min, region_max, val, intervalo_out = max - min;;
    for (channel = 0; channel < in->n_canais; channel++)
    {
        for (row = 0; row < in->altura; row++)
        {
            for (col = 0; col < in->largura; col++)
            {
                val = in->dados [channel][row][col];
                region_min = MIN (img_min->dados [channel][row][col], val);
                region_max = MAX (img_max->dados [channel][row][col], val);

                intervalo_in = region_max - region_min;

                if (intervalo_in == intervalo_out)
                    out->dados [channel][row][col] = val; // Regi�o j� est� normalizada. Fica como est�.
                else
                    out->dados [channel][row][col] = (val - region_min) / intervalo_in * intervalo_out + min; // Normaliza.
            }
        }
    }

    destroiImagem (buffer);
    destroiImagem (img_max);
    destroiImagem (img_min);
    destroiImagem (img_aux);
}

/*----------------------------------------------------------------------------*/
/** Normaliza��o com "clipping". Para evitar problemas causados por valores
 * extremos raros, em vez de pegar simplesmente o m�nimo e o m�ximo, descartamos
 * uma certa porcentagem de valores mais claros e mais escuros. Para isso,
 * montamos um histograma.
 *
 * Par�metros: Imagem* in: imagem de entrada. Se tiver mais que 1 canal,
 *               processa cada canal independentemente.
 *             Imagem* out: imagem de sa�da. Deve ter o mesmo tamanho da
 *               imagem de entrada.
 *             float min: valor inferior da faixa desejada.
 *             float max: valor superior da faixa desejada.
 *             float descartados: propor��o de pixels descartados. Precisa ser
 *               menor que 0.5.
 *
 * Valor de retorno: nenhum. */

void normalizaSemExtremos8bpp (Imagem* in, Imagem* out, float min, float max, float descartados)
{
    if (in->largura != out->largura || in->altura != out->altura || in->n_canais != out->n_canais)
    {
        printf ("ERRO: normalizaSemExtremos8bpp: as imagens precisam ter o mesmo tamanho e numero de canais.\n");
        exit (1);
    }

    if (max <= min)
    {
        printf ("ERRO: normalizaSemExtremos8bpp: max deve ser maior que min.\n");
        exit (1);
    }

    if (descartados <= 0 || descartados >= 0.5f)
    {
        printf ("ERRO: normalizaSemExtremos8bpp: a propor��o de pixels descartados deve ficar no intervalo (0,0.5).\n");
        exit (1);
    }

    int histograma [256];
    int channel, row, col, i;
    float min_in, max_in, intervalo_in, intervalo_out = max - min;
    int n_descartados = (int) (descartados * in->largura * in->altura); // N�mero de pixels "descartados" (ignorados).
    int n_passados;

    // Normaliza os canais da imagem de forma independente.
    for (channel = 0; channel < in->n_canais; channel++)
    {
        criaHistograma8bpp1c (in, channel, histograma); // Cria o histograma para este canal.

        // Agora, procura a faixa de interesse para os valores.
        n_passados = 0;
        for (i = 0; i < 256 && n_passados <= n_descartados; i++)
            n_passados += histograma [i];
        min_in = (i-1)/255.0f;

        n_passados = 0;
        for (i = 255; i >= 0 && n_passados <= n_descartados; i--)
            n_passados += histograma [i];
        max_in = (i+1)/255.0f;

        // Normaliza.
        intervalo_in = max_in - min_in;
        if (intervalo_in < 0.0001f || intervalo_in == intervalo_out)
        {
            for (row = 0; row < in->altura; row++)
                for (col = 0; col < in->largura; col++)
                    out->dados [channel][row][col] = in->dados [channel][row][col]; // Imagem homog�nea ou j� normalizada. Fica como est�.
        }
        else
        {
            for (row = 0; row < in->altura; row++)
                for (col = 0; col < in->largura; col++)
                {
                    float val = in->dados [channel][row][col];
                    if (val <= min_in)
                        out->dados [channel][row][col] = min;
                    else if (val >= max_in)
                        out->dados [channel][row][col] = max;
                    else
                        out->dados [channel][row][col] = (val - min_in) / intervalo_in * intervalo_out + min; // Normaliza.
                }
        }
    }
}

/*============================================================================*/
/* HISTOGRAMAS                                                                */
/*============================================================================*/
/** Cria um histograma de 256 faixas para uma imagem de 1 canal. Para isso, os
 * valores no intervalo [0,1] s�o interpretados como inteiros de 8 bits no
 * intervalo [0,255].
 *
 * Par�metros: Imagem* in: imagem de entrada.
 *             int canal: canal da imagem de entrada a se analisar.
 *             int histograma [256]: histograma de sa�da.
 *
 * Valor de retorno: nenhum (o histograma � preenchido). */

void criaHistograma8bpp1c (Imagem* in, int canal, int histograma [256])
{
    int i;
    for (i = 0; i < 256; i++)
        histograma [i] = 0;

    int row, col;
    unsigned char val;
    for (row = 0; row < in->altura; row++)
        for (col = 0; col < in->largura; col++)
        {
            val = float2uchar (in->dados [canal][row][col]);
            histograma [val]++;
        }
}

/*============================================================================*/
