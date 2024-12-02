#include <stdio.h>
#include <stdlib.h>
#include "abc.h"
/*
  File: abc.h
  Autor: Juan Gago
  Desc: Sesion 46 Academia Opo
*/

int main(void) {
    NODO* raiz = NULL;  /* árbol inicialmente vacío */
    NODO* raiz2 = NULL;  /* árbol inicialmente vacío */
    insertaNodo(&raiz, 6);
    insertaNodo(&raiz, 3);
    insertaNodo(&raiz, 7);
    insertaNodo(&raiz, 2);
    insertaNodo(&raiz, 15);
    insertaNodo(&raiz, 14);
    insertaNodo(&raiz, 1);
    insertaNodo(&raiz, 4);

    printf("\nEl recorrido en inorden es:\n");
    inOrden(raiz);

    printf("\nAhora escribo el árbol en el archivo");
    escribe(raiz,"prueba.txt");

    printf("\nAhora lo leo del archivo y lo pongo en otro árbol\n");
    lee(&raiz2,"prueba.txt");
    inOrden(raiz2);
    printf("\n");
    return 0;
}