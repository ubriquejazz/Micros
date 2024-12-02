#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
 * Date: 2022-03 (sesion 18)
 * File: personas (estructurav3.cpp)
 */

#include <stdio.h>
#include <stdlib.h>

#define NPERSONAS 20

typedef char Cadena_t[20];
typedef char Tlfno_t[9];

typedef union {
	Cadena_t dni;
	Cadena_t nie;
	Cadena_t carnet;
} ID_t;

typedef struct{
	int			cod;
	Cadena_t 	nombre;
	Cadena_t	direccion;
	Tlfno_t		telefono;
//	int     	formaId;
//	ID_t		id;
//	int 		edad;
//	float 		peso;
} Persona_t;

typedef Persona_t Vector_t[NPERSONAS];

void darAlta (Vector_t personas, int* numPersonas, int *cont);
void listado (Vector_t personas, int numPersonas);
void darBaja (Vector_t personas, int* numPersonas);
int buscar(Vector_t personas, int numPersonas, Cadena_t nom, int posInicio);

#endif
