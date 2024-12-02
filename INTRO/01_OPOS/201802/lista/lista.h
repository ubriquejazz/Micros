#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
 * File: lista.h
 * Date: 2023-01 (sesion 43, 13)
 * Book: Oposiciones MAD
 */

typedef struct nodo {
    int value;
    struct  nodo* sig;
} tipoNodo;

typedef tipoNodo* pNodo; 
typedef tipoNodo* Lista;

int SIZE(Lista lista);
void imprimir(Lista lista);
void insertar(Lista* lista, int v);
void eliminar(Lista* lista, int v);

#endif