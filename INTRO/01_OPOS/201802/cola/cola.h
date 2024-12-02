#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
  File: cola.h
  Autor: Juan Gago
*/

typedef struct nodo {
	int value;
	struct nodo *sig;
} tipoNodo;

typedef tipoNodo* pNodo; 
typedef tipoNodo* Cola;

int SIZE(Cola miCola);
int empty(Cola miCola);
int pop(Cola *miCola);
void push(Cola *miCola, int v);

#endif