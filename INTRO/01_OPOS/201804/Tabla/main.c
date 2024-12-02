/*
  File: texto.h
  Autor: Juan Gago
  Desc: Read and write a text file
*/

#include <stdio.h>
#include "texto.h"

int main(void) 
{
	float tabla[4][4][4];
    
	inicializar(tabla);
	printf("Tabla inicial ************\n");
	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			for (int k=0; k<4; k++) {
				printf("%d %d %d %f\n", (i+1), (j+1), (k+1), tabla[i][j][k]);
			}
		}
	}
	cargardatos(tabla);
	printf("Tabla cargada ************\n");
	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			for (int k=0; k<4; k++) {
				printf("%d %d %d %f\n", (i+1), (j+1), (k+1), tabla[i][j][k]);
			}
		}
	}
	tabla[0][0][0]=88;
	almacenardatos(tabla);
	return 0;
}
