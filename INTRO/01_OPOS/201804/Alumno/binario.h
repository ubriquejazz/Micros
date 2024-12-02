#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
  File: binario.h
  Autor: Juan Gago
  Desc: Read and write a binary file
*/

typedef struct alumno {
   int numero;
   int edad;
   char nombre[50];
} tipoAlumno;

void generarFichero(char nombreArchivo[]);  // Crea un nuevo archivo (wb)
void leerFichero(char nombreArchivo[]);     // Abro archivo en modo lectura (rb)

#endif
