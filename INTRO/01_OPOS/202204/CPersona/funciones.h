#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
 * Date: 2022-05
 * File: CPersona (sesion 19)
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Person_s {
	char nombre[50];
	char direccion[50];
	long telefono;
} Person_t;

typedef struct nodo {
    Person_t     valor;
    struct nodo *sig;
} tipoNodo;

typedef tipoNodo *pNodo; 
typedef tipoNodo *Lista;

int ListaTfnos_size(Lista lista);
void ListaTfnos_imprimir(Lista lista);
void ListaTfnos_anadir(Lista *lista, int *nEle, Person_t nuevo);
int ListaTfnos_buscar(Lista lista, int nEle, char nombre[50], int posInicio);

#if 0
void insertarDelante(Lista *lista, Person_t v);
void insertarDetras(Lista *lista, Person_t v);
// insertarOrdenado(Lista *lista, Person_t v);
void insertarDetras(Lista *lista, Person_t v);
void eliminarPrimero(Lista *lista);
void eliminarUltimo(Lista *lista);
// eliminar(Lista *lista, Person_t v);
#endif

#endif
