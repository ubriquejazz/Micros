#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
  File: funciones_linked.c
  Autor: Juan Gago
  Date: 20/10
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int cod;                // Un código consistente en un número consecutivo.
	char titulo[30];        // El título del proyecto.
	char responsable[50];   // El nombre del profesor 
	int unidadGasto;        // La unidad de gasto principal adscrita (un número de 15 cifras)
	float fondos;           // La cuantía de los fondos que tiene concedidos.
	int vigencia;           // Supongo que 0 no vigente y 1 vigente
}tipoValor;

typedef struct nodo{
	tipoValor valor;
	struct nodo* sig;
}tipoNodo;

typedef struct nodo2{
	int valor;
	struct nodo2* sig;
}tipoNodoListaAux;

typedef tipoNodo* Lista;
typedef tipoNodoListaAux* ListaAux;

//Prototipos
void darAlta(Lista*lista, int*cont, tipoValor nuevoValor);
void incrementarFondos(Lista lista, int cod, float cantidad);
void listar1(Lista lista, float limite);
void listar2(Lista lista);

void darAltaDep(ListaAux* listaDpto, int unidadGasto);
int buscarDepartamento(ListaAux listaDpto, int undidadGasto);

//Prototipos
int leerUnidadGasto();
int leerVigencia();
float leerFondos();

#endif