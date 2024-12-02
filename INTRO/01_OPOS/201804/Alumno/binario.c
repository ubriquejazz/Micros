/*
  File: binario.c
  Autor: Juan Gago
  Desc: fwrite and fread a binary file
*/

#include <stdio.h>
#include <stdlib.h>
#include "binario.h"

void generarFichero(char nombreArchivo[])
{
    FILE *f;       // Declaro un puntero al inicio del archivo
    f = fopen(nombreArchivo, "wb");
    if (f == NULL) {
        printf("Error: No pudo abrirse el archivo para escritura.\n");
        return;
    }
    
    char continuar;       // boolean
    tipoAlumno alumno;
    do {
        printf("Datos de un alumno:\n");
        scanf("%d %d %s",&alumno.numero, &alumno.edad, alumno.nombre);
        fflush(stdin);

        //Línea para escribir un registro completo en el archivo
        fwrite(&alumno, sizeof(tipoAlumno), 1, f);
        printf("¿Quieres insertar otro (S/N)?");
        scanf("%c",&continuar);
        while (continuar!='S' && continuar!='N') {
            printf("\nError, repite: ");
            scanf("%c",&continuar);
	    }
    } while (continuar == 'S');
    fclose(f);
}

void leerFichero(char nombreArchivo[])
{
    FILE *f;        //Abro el modo lectura (binario)
    f = fopen(nombreArchivo, "rb");
    if (f == NULL) {
        printf("Error: No pudo abrirse el archivo para lectura.\n");
        return;
    }
    
    printf("Datos de alumnos cargado en el fichero:\n");
  	while(!feof(f)){
        tipoAlumno a;
		if(fread(&a, sizeof(tipoAlumno), 1, f) == 1){
    		printf("%d %d %s\n",a.numero, a.edad, a.nombre);
	  		fflush(stdin);
	  	}  	
	  	else {
	  		break; // para que no muestre dos veces el último registro 
	  	}
	}
  	fclose(f);
}

