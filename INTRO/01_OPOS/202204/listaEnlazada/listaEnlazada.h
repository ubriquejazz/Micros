#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
 * Date: 2022-04
 * File: listas (sesion 13)
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct nodo {
    int     valor;
    struct  nodo *sig;
} tipoNodo;

typedef tipoNodo *pNodo; 
typedef tipoNodo *Lista;

int SIZE(Lista lista);
void imprimirLista(Lista lista);
void insertarDelante(Lista *lista, int v);
void insertarDetras(Lista *lista, int v);
void insertarOrdenado(Lista *lista, int v);
void insertarDetras(Lista *lista, int v);
void eliminarPrimero(Lista *lista);
void eliminarUltimo(Lista *lista);
void eliminar(Lista *lista, int v);

#endif