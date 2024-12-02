#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
 * Date: 2022-03
 * File: impresos (sesion 9)
 */

#include <stdio.h>
#include <stdlib.h>

#define N           100
#define LongCadena  100

typedef char Cadena_t[LongCadena];

typedef struct {
    int         diaIni, diaFin;
} Periodo_t;

typedef struct {
    int         numero;         // valor asignado de forma sucesiva
    Cadena_t    descripcion;
    float       costo;          // cantidad a abonar al adquirir un ejemplar
    Periodo_t   suministro;     // periodo en que se puede suministrar
    int         disponibles;    // ejemplares disponibles en las conserjerías
    int         minimo;         // ejemplares que ha de haber en las conserjerías
} Impreso_t;

typedef Impreso_t Vector_t[N];

void darAlta (Vector_t impresos, int* numImpresos, int* cont);
void listado (Vector_t impresos, int numImpresos);
void darBaja (Vector_t impresos, int numImpresos);
int sumarTotalImpresos (Vector_t impresos, int numImpresos);

#endif