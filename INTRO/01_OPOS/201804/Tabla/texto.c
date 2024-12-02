/*
  File: texto.h
  Autor: Juan Gago
  Desc: fprintf and fscanf a text file
*/

#include <stdio.h>
#include "texto.h"

void inicializar (float tabla[4][4][4])
{
	int i, j, k;
	for (i=0; i<4; i++)
		for (j=0; j<4; j++)
			for (k=0; k<4; k++)
				tabla[i][j][k] = 0;
}

void cargardatos (float tabla[4][4][4])
{
	// estructura de datos - funcion 3D
	int x, y, z;
	float valor;

	FILE *f;
	f = fopen ("datos.txt", "r");
	if (f==NULL)
		printf("Error abriendo el fichero datos.txt");
	else {
		while (!feof(f)) {
			fscanf(f, "%d %d %d %f", &x, &y, &z, &valor);
			tabla[x-1][y-1][z-1] = valor;
		}
		fclose (f);
	}
}

void almacenardatos (float tabla[4][4][4])
{
	FILE *f;
	f = fopen ("datos.txt", "w");
	if (f==NULL)
		printf("Error creadno el fichero datos.txt");
	else {
		printf("Datos que escribo en el archivo:\n");
		int i, j, k;
		for (i=0; i<4; i++){
			for (j=0; j<4; j++){
				for (k=0; k<4; k++){
					if (tabla[i][j][k]!= 0){
						printf("%d %d %d %f\n", (i+1), (j+1), (k+1), tabla[i][j][k]);
						fprintf(f, "%d %d %d %f\n", (i+1), (j+1), (k+1), tabla[i][j][k]);
					}	
				}
			}
		}
	   fclose (f);
	}
}
