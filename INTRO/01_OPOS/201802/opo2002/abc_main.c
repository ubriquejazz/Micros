#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abc.h"

int main(void) {

	Arbol raiz = NULL;  /* árbol inicialmente vacío */
  insertaNodo(&raiz, "Juan", 46);
  insertaNodo(&raiz, "Pepita", 3);
  insertaNodo(&raiz,"Pepe", 79);
  insertaNodo(&raiz,"Ana", 2);
  insertaNodo(&raiz, "Juan", 101);
  insertaNodo(&raiz, "Catalina", 14);
  insertaNodo(&raiz, "Manuel", 70);

  printf("           %s\n",raiz->nombre);
  printf("   %s\n",raiz->izq->nombre);
  printf("                          %s\n",raiz->der->nombre);
  printf("%s\n",raiz->izq->izq->nombre);
  printf("                  %s\n",raiz->der->izq->nombre);
  printf("           %s\n",raiz->izq->der->nombre);
  printf("                                  %s\n",raiz->der->der->nombre);

	printf("\nEl recorrido en inorden es:\n");
	inOrden(raiz);

  printf("\n\nEl recorrido en preorden es:\n");
	preOrden(raiz);

  printf("\n\nEl recorrido en postorden es:\n");
	postOrden(raiz);

  printf("\n\nDatos de los ascentros masculinos:\n");
  datosHombres(raiz);
  printf("\n\nLa edad media de los hombres es %d.\n",edadMediaHombres(raiz));
  printf("\nLa edad media total es %d.\n",edadMedia(raiz));

	return 0;
}