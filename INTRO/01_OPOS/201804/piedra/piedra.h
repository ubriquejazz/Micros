#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
 * Date: 2022-03
 * File: piedras (sesion 8)
 */

#include <stdio.h>
#include <stdlib.h>

#define N 			200
#define LongCadena 	100

typedef struct{
	float longitud;
	float latitud;
} Coord_t;

typedef struct {
	int 	codigo;
	char 	caracterizacion[LongCadena];
	Coord_t ubicacion;
	float 	dureza;
	float 	utilidad;
	int 	porosidad;
} Piedra_t;

typedef Piedra_t Vector_t[N];

void darAlta (Vector_t piedras, int* numPiedras, int* cont);
void introducirAnalisis (Vector_t piedras, int numPiedras);
void listar (Vector_t piedras, int numPiedras);
void obtenerEstadisticas (Vector_t piedras, int numPiedras);

#endif