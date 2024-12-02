/*
 * Date: 2022-03
 * File: impresos (sesion 9)
 */

#include <stdio.h>
#include <stdlib.h>
#include "impresos.h"

void mostrarMenu();
int leerOpcion();

int main() {
	Vector_t impresos;
	int numImpresos = 0;
	int opcion, cont=1, total=0;

	do {
		mostrarMenu();
		opcion = leerOpcion();
        switch(opcion) 
        {
            case 1: 
                darAlta(impresos, &numImpresos, &cont);
                break;
            case 2: 
                listado(impresos, numImpresos);
                break;
            case 3: 
                darBaja(impresos, numImpresos);
                break;
            case 4: 
                total = sumarTotalImpresos(impresos);
                printf('El numero total de impresos es %d', total);
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
	printf("\n\tPulse 1 para dar de alta un nuevo tipo de impreso.");
	printf("\n\tPulse 2 para obtener un listado de los impresos bajo mínimos.");
	printf("\n\tPulse 3 para dar de baja impresos de un tipo.");
	printf("\n\tPulse 4 para obtener el valor conjunto de todos los impresos.");
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
