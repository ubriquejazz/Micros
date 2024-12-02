#include <stdio.h>
#include <stdlib.h>
#include "piedra.h"

void mostrarMenu();
int leerOpcion();

int main() {
	Vector_t piedras;
	int numPiedras = 0;
	int opcion, cont=1;

	do {
		mostrarMenu();
		opcion = leerOpcion();
		switch (opcion) {
			case 1:
				darAlta(piedras, &numPiedras, &cont);
				break;
			case 2:
				introducirAnalisis(piedras, &numPiedras);
				break;
			case 3:
				listar(piedras, numPiedras);
				break;
			case 4:
				obtenerEstadisticas(piedras, numPiedras);
				break;
			default:
				printf("Bye");
				break;
		}
	} while (opcion != 5);
	return 0;
}

void mostrarMenu()
{
	printf("\nMenú de opciones:");
	printf("\n\tPulse 1 para dar de alta.");
	printf("\n\tPulse 2 para introducir analisis.");
	printf("\n\tPulse 3 para listar.");
	printf("\n\tPulse 4 para obtener estadisticas.");
	printf("\n\tPulse 5 para salir.");
	printf("\n\tElija opion: ");
}

int leerOpcion()
{
	int opcion;
	scanf("%d",&opcion);
	while(opcion<1 || opcion >5){
		printf("Mal, repite: ");
		scanf("%d",&opcion);
	}
	return opcion;
}