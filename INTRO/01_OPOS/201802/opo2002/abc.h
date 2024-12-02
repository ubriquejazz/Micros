#ifndef ABC_H
#define ABC_H

/*
  File: funciones.h
  Autor: Juan Gago
*/

typedef struct nodoArbol {
	char  nombre[50];
	int   edad;
	struct nodoArbol* izq;	// mama
	struct nodoArbol* der;	// papa
}tipoNodoArbol;

typedef tipoNodoArbol* pNodo;
typedef tipoNodoArbol* Arbol;

typedef struct nodoCola {
	Arbol valor;
	struct nodoCola* sig;
}tipoNodoCola;

typedef tipoNodoCola* Cola;

//Prototipos sobre la cola
int colaVacia(Cola miCola);
void push(Cola* miCola, Arbol nodo);
Arbol pop(Cola* miCola);

//Prototipo de procedimiento que os doy
void insertaNodo(Arbol*, char nombre[], int edad);

int datosHombres(Arbol raiz);
int edadMediaHombres(Arbol raiz);
int edadMedia(Arbol raiz);
void inOrden(Arbol raiz);
void preOrden(Arbol raiz);
void postOrden(Arbol raiz);

#endif