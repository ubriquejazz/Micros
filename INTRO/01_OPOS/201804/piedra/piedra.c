/*
 * Date: 2022-03
 * File: piedras (sesion 8)
 */

#include <stdio.h>
#include <stdlib.h>
#include "piedra.h"

void darAlta(Vector_t piedras, int* numPiedras, int* cont) {
	Piedra_t nueva;
	nueva.codigo	=*cont;
	nueva.dureza	=-1;
	nueva.utilidad	=-1;
	nueva.porosidad	=-1;

	if(*numPiedras == N){
		printf("No caben más.");
	}
	else {
		printf("Dime la caracterización geológica: ");
		scanf("%s",nueva.caracterizacion);

		printf("Dame longitud: ");
		scanf("%f",&nueva.ubicacion.longitud);
		while(nueva.ubicacion.longitud<-180 || nueva.ubicacion.longitud>180){
			printf("Error, repite: ");
			scanf("%f",&nueva.ubicacion.longitud);
		}
		printf("Dame latitud: ");
		scanf("%f",&nueva.ubicacion.latitud);
		while(nueva.ubicacion.latitud<-90 || nueva.ubicacion.latitud>90){
			printf("Error, repite: ");
			scanf("%f",&nueva.ubicacion.latitud);
		}
		piedras[*numPiedras]=nueva;
		(*numPiedras)++;
		(*cont)++;
		printf("La piedra se ha añadido correctamente con código igual a %d.\n", nueva.codigo);
	}
}

void introducirAnalisis(Vector_t piedras, int numPiedras) {
	int c;
	int pos = -1;

	printf("Dime el código de la piedra a introducir datos: ");
	scanf("%d",&c);
  for(int i=0; i<numPiedras && pos == -1; i++){
		if(piedras[i].codigo==c){
			pos = i;
		}
	}
	if(pos == -1){
		printf("Esa piedra no la tenemos.\n");
	}else{
		printf("Dame la dureza: ");
		scanf("%f",&piedras[pos].dureza);
		while(piedras[pos].dureza<=0){
			printf("Mal repite: ");
			scanf("%f",&piedras[pos].dureza);
		}
		printf("Dame la utilidad: ");
		scanf("%f",&piedras[pos].utilidad);
		while(piedras[pos].utilidad<0 || piedras[pos].utilidad>1 ){
			printf("Mal repite: ");
			scanf("%f",&piedras[pos].utilidad);
		}
		printf("Dame la porosidad: ");
		scanf("%d",&piedras[pos].porosidad);
		while(piedras[pos].porosidad<0 || piedras[pos].porosidad>100 ){
			printf("Mal repite: ");
			scanf("%d",&piedras[pos].porosidad);
		}
	}
}

void listar(Vector_t piedras, int numPiedras)
{
	for (int i=0; i<numPiedras; i++) {
		printf("Mostrando info sobre la piedra con código %d\n",piedras[i].codigo);
		printf("- caracterización geológica: %s\n", piedras[i].caracterizacion);
		Coord_t loc = piedras[i].ubicacion;
		printf("- ubicacion: %f, %f\n", loc.latitud, loc.longitud);
		printf("- dureza: %f\n", piedras[i].dureza);


		printf("- utilidad: %f\n", piedras[i].utilidad);
		printf("- porosidad: %d\n", piedras[i].porosidad);
	}
}

void obtenerEstadisticas(Vector_t piedras, int numPiedras)
{
	float porcentage;
	int contPendientes = 0;
	int contUtilidadPos =0;
	int contUtilidadNeg =0;

	// tipos de piedra q están pendientes de análisis
	for (int i=0; i<numPiedras; i++) {
		if (piedras[i].utilidad == -1) {
			contPendientes++;
		}
		else if (piedras[i].utilidad > 0.5) {
			contUtilidadPos++;
		}
		else {
			contUtilidadNeg++;
		}
	}
	porcentage = 100 * (float)contPendientes/ (float)numPiedras;
	printf("Hay pendientes de analisis %d piedras, %.2f%%\n", contPendientes, porcentage);
	
	porcentage = 100 * (float)contUtilidadPos/ (float)numPiedras;
	printf("Hay analizadas con positivo %d piedras, %.2f%%\n", contUtilidadPos, porcentage);

	porcentage = 100 * (float)contUtilidadNeg/ (float)numPiedras;
	printf("Hay analizadas con negativo %d piedras, %.2f%%\n", contUtilidadNeg, porcentage);
}