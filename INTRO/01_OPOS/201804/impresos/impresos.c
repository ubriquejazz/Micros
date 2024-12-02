/*
 * Date: 2022-03
 * File: impresos (sesion 9)
 */

#include <stdio.h>
#include <stdlib.h>
#include "impresos.h"

void darBaja(Vector_t impresos,int numImpresos)
{
	int num, cant, pos=-1;
	printf("Dime el número de impreso del que quitar elementos: ");
	scanf("%d",&num);
	for(int i=0; i<numImpresos && pos==-1; i++){
		if(impresos[i].numero == num)
			pos = i;
	}
	if(pos==-1) 
		printf("Ese impreso no está");
    else {
		printf("Dime la cantidad a sacar: ");
		scanf("%d",&cant);
		if(impresos[pos].disponibles < cant)
			impresos[pos].disponibles = 0;
		else
			impresos[pos].disponibles = impresos[pos].disponibles-cant;
	}
}

int sumarTotalImpresos(Vector_t impresos,int numImpresos)
{
	int sumatoria=0;
	for(int i=0; i<numImpresos;i++){
		sumatoria += impresos[i].disponibles;
	}	
	return sumatoria;
}

void listado (Vector_t impresos,int numImpresos)
{
    int cont=0;
	for(int i=0; i<numImpresos; i++){
		if(impresos[i].disponibles < impresos[i].min) {
			printf("\n%d (%s): %d-%d", 
                impresos[i].numero,
                impresos[i].descripcion,
                impresos[i].disponibles,
                impresos[i].min);  
            cont++;
        }
	}
    printf("\nImpresos bajo minimos: %d", cont);
}

void darAlta(Vector_t impresos, int* numImpresos, int* cont)
{
	if (*numImpresos == N)
		printf("Vector lleno.");
    else 
	{
		impresos[*numImpresos].numero = *cont;
		printf("Da descripcion: ");
		scanf("%s",impresos[*numImpresos].descripcion);
		printf("Da costo: ");
		scanf("%f",&impresos[*numImpresos].costo);
		while(impresos[*numImpresos].costo<=0){
			printf("Error, repite: ");
			scanf("%f",&impresos[*numImpresos].costo);
		}
		printf("Da dia inicio: ");
		scanf("%d",&impresos[*numImpresos].suministro.diaIni);
		while(impresos[*numImpresos].suministro.diaIni<1 || impresos[*numImpresos].suministro.diaIni>365){
			printf("Error, repite: ");
			scanf("%d",&impresos[*numImpresos].suministro.diaIni);
		}
		printf("Da dia fin: ");
		scanf("%d",&impresos[*numImpresos].suministro.diaFin);
		while(impresos[*numImpresos].suministro.diaFin<1 || impresos[*numImpresos].suministro.diaFin>365){
			printf("Error, repite: ");
			scanf("%d",&impresos[*numImpresos].suministro.diaFin);
		}
		printf("Da disponibles: ");
		scanf("%d",&impresos[*numImpresos].disponibles);
		while(impresos[*numImpresos].disponibles<1){
			printf("Error, repite: ");
			scanf("%d",&impresos[*numImpresos].disponibles);
		}
		printf("Da minimo: ");
		scanf("%d",&impresos[*numImpresos].min);
		while(impresos[*numImpresos].min<1){
			printf("Error, repite: ");
			scanf("%d",&impresos[*numImpresos].min);
		}
		(*numImpresos)++;
		(*cont)++;
	}
}
