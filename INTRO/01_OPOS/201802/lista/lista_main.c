#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

int main(void) {
    int elemento;
	Lista raiz = NULL;  /* árbol inicialmente vacío */
    printf("Lista de %d elementos\n", SIZE(raiz));
    insertar(&raiz, 0);
    insertar(&raiz, 1);
    insertar(&raiz, 5);
    insertar(&raiz, 7);
    printf("Lista de %d elementos\n", SIZE(raiz));
    eliminar(&raiz, 0);
    imprimir(raiz);
    printf("\nLista de %d elementos\n", SIZE(raiz));
    return 0;
}