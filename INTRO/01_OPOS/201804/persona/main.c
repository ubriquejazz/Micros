#include <stdio.h>
#include "persona.h"

void mostrarMenu();
int leerOpcion();

int main() {
	Vector_t personas;
	Cadena_t busqueda;
	int numPersonas = 0;
	int opcion, cont=1;

	do {
	mostrarMenu();
	opcion = leerOpcion();
        switch(opcion) 
        {
            case 1: 
                darAlta(personas, &numPersonas, &cont);
                break;
            case 2: 
                listado(personas, numPersonas);
                break;
            case 3: 
                darBaja(personas, &numPersonas);
                break;
            case 4: 
		printf("Nombre: ");
		scanf("%s", busqueda);
		int pos = buscar(personas, numPersonas, busqueda, 0); 
		printf("Numero %d de %d personas\n", pos, numPersonas);
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
	printf("\n\tPulse 2 para obtener un listado.");
	printf("\n\tPulse 3 para dar de baja.");
	printf("\n\tPulse 4 para debugado.");
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
