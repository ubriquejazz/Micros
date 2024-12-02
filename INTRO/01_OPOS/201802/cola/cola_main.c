#include <stdio.h>
#include <stdlib.h>
#include "cola.h"

int main() {
    int element;
    Cola raiz = NULL;  /* árbol inicialmente vacío */
    printf ("Hay %d elementos\n", SIZE(raiz));

    push(&raiz, 0);
    push(&raiz, 1);
    push(&raiz, 2);
    push(&raiz, 3);
    printf ("Hay %d elementos\n", SIZE(raiz));
    while (empty(raiz)==0) {
        element = pop(&raiz);
        printf ("%d\n", element);
    }

    printf ("Hay %d elementos\n", SIZE(raiz));
    return 0;
}