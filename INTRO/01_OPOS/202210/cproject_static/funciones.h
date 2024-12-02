#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
  File: funciones_static.c
  Autor: Juan Gago
  Date: 22/10
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define N 20

typedef struct{
	int cod;                // Un código consistente en un número consecutivo.
	char titulo[30];        // El título del proyecto.
	char responsable[50];   // El nombre del profesor 
	int unidadGasto;        // La unidad de gasto principal adscrita (un número de 15 cifras)
	float fondos;           // La cuantía de los fondos que tiene concedidos.
	int vigencia;           // Supongo que 0 no vigente y 1 vigente
}tipoValor;

typedef tipoValor tipoVector[N];

typedef struct nodo2{
	int valor;
	struct nodo2* sig;
}tipoNodoListaAux;

typedef tipoNodoListaAux* ListaAux;

//Prototipos
void darAlta(tipoVector vector, int*numElementos, int*cont, tipoValor nuevoValor);
void incrementarFondos(tipoVector vector, int numElementos, int cod, float cantidad);
void listar1(tipoVector vector, int numElementos, float limite);
void listar2(tipoVector vector, int numElementos);

void darAltaDep(ListaAux* listaDpto, int unidadGasto);
int buscarDepartamento(ListaAux listaDpto, int undidadGasto);

//Prototipos
int leerUnidadGasto();
int leerVigencia();
float leerFondos();

#endif