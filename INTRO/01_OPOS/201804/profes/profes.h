#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
 * Date: 2022-03
 * File: profes.c (sesion 10 y 7)
 */

#include <stdio.h>

#define NPROFES 20
#define DNI     0

typedef union {
    char dni[9];
    char pasaporte[12];
} Id_t;

typedef struct 
{
    int     cod;        // 
    int     tipoId;     // 0 si dni, 1 si pasaporte 
    Id_t    id;         //
    char    nombre[50]; // Nombre del profesor
    int     nHoras;     // Horas que va a impartir clase (de 1 a 100)
    int     diaComienzo; // Día en que comienza sus clases (del 1 al 30).
} Profe_t;

typedef Profe_t Vector_t[NPROFES];

void mostrarProfesor(Profe_t profesor);
void darAlta (Vector_t profesores, int* numProfes, int *cont);
void darBaja (Vector_t profesores, int* numProfes);
void listadoProfes (Vector_t profesores, int numProfes);
void ordenarProfes (Vector_t profesores, int numProfes);
void listadoProfesHoras (Vector_t profesores, int numProfes);

#endif