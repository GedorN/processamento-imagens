/*============================================================================*/
/* Exemplo: segmenta��o de uma imagem em escala de cinza.                     */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu                                                     */
/* Universidade Tecnol�gica Federal do Paran�                                 */
/*============================================================================*/

#include <iostream>
#include <time.h>
#include <vector>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

/*============================================================================*/

#define INPUT_IMAGE "arroz.bmp"

// TODO: ajuste estes par�metros!
#define NEGATIVO false // aroz: false; texto: true
#define THRESHOLD 0.81f // arroz: 0.81f; texto: 0.53f
#define ALTURA_MIN 5
#define LARGURA_MIN 5
#define N_PIXELS_MIN 5

/*============================================================================*/

typedef struct
{
    float label; // R�tulo do componente.
    Rect roi; // Coordenadas do ret�ngulo envolvente do componente.
    int n_pixels; // N�mero de pixels do componente.

} Componente;

/*============================================================================*/

void binariza (Mat& in, Mat& out, float threshold);
void rotula (Mat& img, vector <Componente>& componentes, int largura_min, int altura_min, int n_pixels_min);
void inunda (Mat& img, Componente &componente, int row, int col);
/*============================================================================*/

int main ()
{
    // Abre a imagem em escala de cinza, e mant�m uma c�pia colorida dela para desenhar a sa�da.
    Mat img = imread (INPUT_IMAGE, IMREAD_GRAYSCALE);
    Mat aux = img.clone();
    Rect roi;
    if (!img.data)
    {
        cerr << "Erro abrindo a imagem.\n";
        exit (1);
    }
	// Converte para float.
    img.convertTo(aux, CV_32F, 1.0/255.0);
    Mat img_out (img.rows, img.cols, CV_8UC3);
    cvtColor (img, img_out, COLOR_GRAY2BGR);

    // Segmenta a imagem.
    if (NEGATIVO)
        aux = 1 - aux;

    binariza (aux, img_out, THRESHOLD);
	imwrite ("01 - binarizada.png", img_out);
    imshow ("01 - binarizada", img_out);

    vector <Componente> componentes;
    clock_t tempo_inicio = clock ();
    rotula (aux, componentes, LARGURA_MIN, ALTURA_MIN, N_PIXELS_MIN);
    clock_t tempo_total = clock () - tempo_inicio;

    cout << "Tempo: " << (int) tempo_total << endl;
    cout << componentes.size () << " componentes detectados" << endl;

    // // Mostra os objetos encontrados.
    cvtColor(img, img, 3, 3);
    for (unsigned int i = 0; i < componentes.size (); i++) {
        rectangle(img, componentes[i].roi, Scalar(0, 0 , 255));
    }
	imshow ("02 - out", img);
	imwrite ("02 - out.bmp", img);
	waitKey ();

    return (0);
}

/*----------------------------------------------------------------------------*/
/** Binariza��o simples por limiariza��o.
 *
 * Par�metros: Mat& in: imagem de entrada. Se tiver mais que 1 canal, binariza
 *               cada canal independentemente.
 *             Mat& out: imagem de sa�da. Deve ter o mesmo tamanho da imagem de
 *               entrada.
 *             float threshold: limiar.
 *
 * Valor de retorno: nenhum (usa a imagem de sa�da). */

void binariza (Mat& in, Mat& out, float threshold)
{
    if (in.cols != out.cols || in.rows != out.rows)
    {
        cerr << "ERRO: binariza: as imagens precisam ter o mesmo tamanho e numero de canais.\n";
        exit (1);
    }

    // TODO: escreva o c�digo desta fun��o.
    for (int i = 0; i < in.rows; i++) {
        for (int j = 0; j < in.cols ; j++) {
            Vec3b &pixel_out = out.at<Vec3b>(i,j); 
            float &pixel = in.at<float>(i,j); 
            float value = pixel;
            if (value > threshold) {
                pixel_out.val[0] = pixel_out.val[1] = pixel_out.val[2] = 255;
                pixel = 1;
            } else {
                pixel_out.val[0] = pixel_out.val[1] = pixel_out.val[2] = 0;
                pixel = 0;
            }
            
        }
    }
    

}

/*============================================================================*/
/* ROTULAGEM                                                                  */
/*============================================================================*/
/** Rotulagem usando flood fill. Marca os objetos da imagem com os valores
 * [0.1,0.2,etc].
 *
 * Par�metros: Mat& img: imagem de entrada E sa�da.
 *             vector <Componente>& componentes: coloca aqui os componentes
 *               gerados.
 *             int largura_min: descarta componentes com largura menor que esta.
 *             int altura_min: descarta componentes com altura menor que esta.
 *             int n_pixels_min: descarta componentes com menos pixels que isso.
 *
 * Valor de retorno: nenhum. Usa o vetor de sa�da. */

void rotula (Mat& img, vector <Componente>& componentes, int largura_min, int altura_min, int n_pixels_min)
{
    // TODO: escreva esta fun��o.
	// Use a abordagem com flood fill recursivo.
    int label  = 0;
    for(int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            float &pixel = img.at<float>(i,j);
            if (pixel == 1) {
                Componente componente;
                componente.n_pixels = 0;
                componente.roi.width = 1;
                componente.roi.height = 1;
                componente.roi.x = j;
                componente.roi.y = i;
                componente.label = ++label;
                inunda(img, componente, i, j);
                if (componente.n_pixels > n_pixels_min && componente.roi.width > largura_min && componente.roi.height > altura_min) {
                    componentes.push_back(componente);
                } else {
                    label--;
                }
            }
        }
    }
}

void inunda (Mat& img, Componente &componente, int row, int col) {
        componente.n_pixels++;

        if (row < componente.roi.y) {
            int height = componente.roi.height  + abs(componente.roi.y - row);
            componente.roi.height = height;
            componente.roi.y = row;
        }
        if (col < componente.roi.x) {
            int width = componente.roi.width  + abs(componente.roi.x - col);
            componente.roi.width = width;
            componente.roi.x = col;
        }


        int width = abs(col - componente.roi.x) + 1;
        int height = abs(row - componente.roi.y) + 1;
        if (componente.roi.height < height) {
            componente.roi.height = height;
        }

        if (componente.roi.width < width) {
            componente.roi.width = width;
        }


        img.at<float>(row, col) = 0.5;
        if (img.at<float>(row +1, col) == 1 && (row + 1 ) < img.rows) {
            inunda(img, componente, row + 1, col);
        }
        if (img.at<float>(row - 1, col) == 1 && (row - 1 ) > 0) {
            inunda(img, componente, row - 1, col);
        }
        if (img.at<float>(row, col + 1) == 1 && (col + 1 ) < img.cols) {
            inunda(img, componente, row, col + 1);
        }
        if (img.at<float>(row, col - 1) == 1 && (col - 1 ) > 0) {
            inunda(img, componente, row, col - 1);
        }

}




/*============================================================================*/
