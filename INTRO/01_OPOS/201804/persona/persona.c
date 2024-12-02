/*
 * Date: 2022-03 (sesion 8)
 * File: personas (estructurav3.cpp)
 */
#include <stdio.h>
#include <string.h>
#include "persona.h"

void darAlta (Vector_t personas, int* numPersonas, int *cont)
{
	if (*numPersonas == NPERSONAS)
        printf("Vector lleno.");
    else 
    {
      	personas[*numPersonas].cod = *cont;
	//	printf("¿Cómo te identificas (0-DNI,1-NIE,2-Carnet)? ");
	//	scanf("%d",&personas[*numPersonas].formaId);
	//	switch(personas[*numPersonas].formaId) {
	//		case 0:
	//			printf("Dime tu dni: ");
	//			scanf("%s", personas[*numPersonas].id.dni);
	//			break;
	//		case 1:
	//			printf("Dime tu nie: ");
	//			scanf("%s", personas[*numPersonas].id.nie);
	//			break;
	//		default:
	//			printf("Dime tu carnet: ");
	//			scanf("%s", personas[*numPersonas].id.carnet);
	//			break;
	//	}
		printf("Nombre: ");
		scanf("%s", personas[*numPersonas].nombre);
		printf("Telefono: ");
		scanf("%s", personas[*numPersonas].telefono);
	//	printf("Edad: ");
	//	scanf("%d", &personas[*numPersonas].edad);
	//	printf("Peso: ");
	//	scanf("%f", &personas[*numPersonas].peso);
		(*cont)++;
		(*numPersonas)++;
	}
}

void listado (Vector_t personas, int numPersonas) {
	for(int i=0; i<numPersonas; i++){
		printf("Codigo %d (%s) vive %s \n", personas[i].cod, 
				personas[i].nombre, personas[i].direccion);
		// ("Codigo %d (%s) tiene %d años y pesa %.2f \n",
		//		personas[i].cod, personas[i].nombre, 
		//		personas[i].edad, personas[i].peso);
	}
}

void darBaja(Vector_t personas, int* numPersonas) {
	int num, i; 
	int pos=-1;
	printf("Dime el número de la persona: ");
	scanf("%d",&num);
	for(i=0; i<*numPersonas && pos==-1; i++){
		if(personas[i].cod == num)
			pos = i;
	}
	if(pos==-1) 
		printf("Esa personas no está\n");
    else {
		for(int i=pos; i < (*numPersonas) - 1; i++) {
            personas[i]=personas[i+1];
        }
        (*numPersonas)--;
	}
}

int buscar(Vector_t personas, int numPersonas, Cadena_t nom, int posInicio) {
	int retVal = -1;
	for(int i=0; i<numPersonas; i++){
		if (strcmp (personas[i].nombre, nom) == 0) {
			retVal = i;
		}	
	}
	return retVal;
}
