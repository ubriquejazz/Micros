/*
  File: main.c (linked list)
  Autor: Juan Gago
  Date: 22/10 (sesion 32)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

void mostrarMenu(){
	printf("\n** Menu **");
	printf("\n\tPulse 1 para dar de alta un nuevo proyecto.");
	printf("\n\tPulse 2 para incrementar los fondos de un proyecto.");
	printf("\n\tPulse 3 para listar los proyectos superiores a una cantidad de fondos.");
	printf("\n\tPulse 4 para listar los departamentos (unidades de gasto).");
	printf("\n\tPulse 5 para salir.");
	printf("\n\tElija opion: ");
}

int leerOpcion(){
	int opcion;
	scanf("%d",&opcion);
	while(opcion<1 || opcion >5){
		printf("Mal, repite: ");
		scanf("%d",&opcion);
	}
	return opcion;
}

int main(){
	int contador=1;
	Lista lista=NULL;
	int opcion;
	char titulo[30];
	char responsable[50];
	int unidadGasto;
	float fondos;
	int vigencia;
	tipoValor nuevo;
	int codigo;
	
	do{
		mostrarMenu();
		opcion=leerOpcion();
		switch(opcion){
			case 1: 
				printf("Dame el titulo: ");
				scanf("%s",nuevo.titulo);
				printf("Dame el responsable: ");
				scanf("%s",nuevo.responsable);
				printf("Dime la unidad de gasto: ");
				nuevo.unidadGasto = leerUnidadGasto();
				printf("Dime los fondos: ");
				nuevo.fondos = leerFondos();
				printf("Dime la vigencia (0 no vigente - 1 vigente): ");
				nuevo.vigencia = leerVigencia();
				darAlta(&lista, &contador,nuevo);
			    break;
			case 2:
				printf("Dime el código del proyecto: ");
				scanf("%d",&codigo);
				printf("Dime la cantidad: ");
				fondos = leerFondos();
				incrementarFondos(lista, codigo, fondos); 
			    break;
			case 3: 
				printf("Dime la cantidad limite: ");
				fondos = leerFondos();
				listar1(lista, fondos);
			    break;
			case 4: 
				listar2(lista);
			    break;
			default: printf("BYE BYE\n");
		}
		
	}while(opcion!=5);
	
	return 0;
}