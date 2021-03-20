#include <stdio.h>


void pirntMatrix(int array[][4]){
    for (int i = 0; i < 4 ; i ++) {
        for (int j = 0; j < 4; j++) {
            printf("%4d", array[i][j]);
        }

        printf("\n");
    }
}
int main() {
    int array[4][4] = {{1,2,3,4}, {5, 6, 7, 8}, {9,10,11,12}, {13, 14,15,16}};
    int aux[4][4];
    for (int i = 0; i < 4; i++) {
        if (i != 0) {
            aux[i][0] = array[i][0] + aux[i - 1][0];
        } else {
            aux[0][0] = array[0][0];
            for (int j = 1; j < 4; j++) {
                aux[i][j] = array[i][j] + aux[i][j - 1];
            }
        }
        
    }

    for (int i = 1; i < 4; i++) {
        for (int j = 1; j < 4; j++) {
            aux[i][j] =  array[i][j] + aux[i][j - 1]+ aux[i - 1][j] - aux[i - 1][j - 1];
        }
    }

    pirntMatrix(aux);

    return 0;

}