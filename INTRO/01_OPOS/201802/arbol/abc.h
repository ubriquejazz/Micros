#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
  File: funciones.h
  Autor: Juan Gago
*/

typedef struct nodoArbol {
	int valor;
	struct nodoArbol *izq;
	struct nodoArbol *der;
} tipoNodoArbol;

typedef tipoNodoArbol* Arbol;

typedef struct nodoCola {
	Arbol valor;
	struct nodoCola *sig;
} tipoNodoCola;

typedef tipoNodoCola* Cola;
typedef tipoNodoCola* pNodo;

int SIZE(Cola miCola);
int empty(Cola miCola);
void push(Cola* miCola, Arbol nodo);
Arbol pop(Cola* miCola);
void liberar(Cola* miCola);

void insertar(Arbol* raiz, int valor);
Arbol buscar(Arbol raiz, int valor);

void preOrden(Arbol raiz);
void inOrden(Arbol raiz);
void postOrden(Arbol raiz);

#endif