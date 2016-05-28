#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 40

int *interleaving(int vetor[], int tam) {
    int *vetor_auxiliar;
    int i1, i2, i_aux;

    vetor_auxiliar = (int *) malloc(sizeof(int) * tam);

    i1 = 0;
    i2 = tam / 2;

    for (i_aux = 0; i_aux < tam; i_aux++) {
        if (((vetor[i1] <= vetor[i2]) && (i1 < (tam / 2)))
            || (i2 == tam))
            vetor_auxiliar[i_aux] = vetor[i1++];
        else
            vetor_auxiliar[i_aux] = vetor[i2++];
    }

    return vetor_auxiliar;
}

int main() {
    int vetor[ARRAY_SIZE];
    int *vetor2;
    int i;

    for (i = 0; i < ARRAY_SIZE / 2; i++)              /* init array with worst case for sorting */
        vetor[i] = i;
    for (i = ARRAY_SIZE / 2; i < ARRAY_SIZE; i++)              /* init array with worst case for sorting */
        vetor[i] = i - ARRAY_SIZE / 2;

    printf("\nVetor: ");
    for (i = 0; i < ARRAY_SIZE; i++)              /* print unsorted array */
        printf("[%03d] ", vetor[i]);

    vetor2 = interleaving(vetor, ARRAY_SIZE);


    printf("\nVetor: ");
    for (i = 0; i < ARRAY_SIZE; i++)                              /* print sorted array */
        printf("[%03d] ", vetor2[i]);

    return 0;
}

