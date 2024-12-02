#include <stdio.h>
#include <stdlib.h>
#include "abc.h"

int main(void) {
	Arbol raiz = NULL;  /* árbol inicialmente vacío */
	printf("Inserto (anchura) 6, ");
	insertar(&raiz, 6);
		printf("3, ");
	insertar(&raiz, 3);
		printf("7, ");
	insertar(&raiz, 7);
		printf("2, ");
	insertar(&raiz, 2);

	printf("\nizdo > raiz > dcho: "); inOrden(raiz);
	printf("\nraiz > izdo > dcho: "); preOrden(raiz);
	printf("\nizdo > dcho > raiz: "); postOrden(raiz);

	int elemento = 3;
	printf("\nVoy a buscar el elemento %d: ", elemento);
	Arbol nodo = buscar(raiz, elemento);
	if (nodo != NULL) {
		printf("%u\n", (unsigned int)nodo);
		printf("%u\n", (unsigned int)nodo->izq);
	}
	return 0;
}

void kk() {
	Arbol raiz = NULL;  /* árbol inicialmente vacío */
		printf("14, ");
	insertar(&raiz, 14);
		printf("1, ");
	insertar(&raiz, 1);
		printf("4");
	insertar(&raiz, 4);
}