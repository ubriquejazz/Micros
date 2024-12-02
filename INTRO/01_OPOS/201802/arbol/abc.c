#include <stdio.h>
#include <stdlib.h>
#include "abc.h"

int SIZE(Cola miCola) {
	int num=0;
    pNodo aux = miCola; 
    while(aux!=NULL){
        num++;
        aux = aux->sig;
    }
    return num;
}

int empty(Cola miCola){
	if(miCola == NULL)
		return 1;
	else
		return 0;
}

Arbol pop(Cola *miCola){
	Arbol nodo=NULL;
	if(*miCola == NULL)
		return NULL;
	else {
		nodo = (*miCola)->valor;
		Cola aux = *miCola;
		(*miCola)=(*miCola)->sig;
		free(aux);
	}
	return nodo;
}

void liberar(Cola *miCola) {
//	while (miCola == NULL) {
//		pop(miCola);
//	}
}

void push(Cola *miCola, Arbol nodo) {
	if(*miCola == NULL) {
		*miCola = (Cola)malloc(sizeof(tipoNodoCola));
		if (*miCola != NULL)
		{
			(*miCola)->valor = nodo;
			(*miCola)->sig = NULL;
		}
	}
	else{
		Cola aux = *miCola;
		Cola nodoCola;
		while(aux->sig != NULL){
			aux=aux->sig;
		}
		nodoCola = (Cola)malloc(sizeof(tipoNodoCola));
		nodoCola->valor = nodo;
		nodoCola->sig = NULL;
		aux->sig = nodoCola;
	}
}

/* ARBOL - estructura autoreferenciada */

Arbol buscar(Arbol raiz, int valor) {
	if (raiz == NULL)
		return NULL; // arbol vacio
	if (raiz->valor == valor)
		return raiz; // encontrado al principio

	Cola miCola = NULL;
	if (raiz->izq != NULL)
		push(&miCola, raiz->izq);
	else if (raiz->der != NULL)
		push(&miCola, raiz->der);
	
	Arbol nodo;
	while (empty(miCola) == 0) {
		nodo = pop(&miCola);
		if(nodo->valor == valor) 
			return nodo; // encontrado!
		if(nodo->izq != NULL)
			push(&miCola, nodo->izq);
		else if(nodo->der != NULL)
			push(&miCola,  nodo->der);
	}
	return NULL; // no encontrado
}

void insertar_helper(Arbol *raiz, int valor) {
	Cola miCola = NULL;
	push(&miCola,(*raiz)->izq); // izdo
	push(&miCola,(*raiz)->der); // dcho

	Arbol nodo;
	while(empty(miCola) == 0){
		nodo = pop(&miCola);
		if(nodo->izq == NULL){
			insertar(&(nodo->izq), valor);
			break;
		}
		else if(nodo->der == NULL){
			insertar(&(nodo->der), valor);
			break;
		}
		else { 
			push(&miCola, nodo->izq);
			push(&miCola, nodo->der);
		}
	} 
}

void insertar(Arbol *raiz, int valor) {
	if (*raiz == NULL) {
		*raiz = (Arbol)malloc(sizeof(tipoNodoArbol));
		if (*raiz != NULL) {
			(*raiz)->valor = valor;
			(*raiz)->izq = NULL;
			(*raiz)->der = NULL;
		} 
	}
	else {
		if ((*raiz)->izq == NULL) {
			insertar(&((*raiz)->izq), valor);
		}
		else if ((*raiz)->der == NULL) {
			insertar(&((*raiz)->der), valor);
		}
		else {
			Cola miCola = NULL;
			push(&miCola, (*raiz)->izq);
			push(&miCola, (*raiz)->der);
			Arbol nodo;
			while (empty(miCola) == 0) {
				nodo = pop(&miCola);
				if(nodo->izq == NULL){
					insertar(&(nodo->izq), valor);
					break;
				}
				else if(nodo->der == NULL){
					insertar(&(nodo->der), valor);
					break;
				}
				else {
					push(&miCola,(nodo->izq));
					push(&miCola,(nodo->der));
				}
			} 
		}
	}
}

Arbol addRecursive(Arbol *raiz, int valor) {
	if(*raiz == NULL) {
		*raiz = (Arbol)malloc(sizeof(tipoNodoArbol));
		if (*raiz != NULL) {
			(*raiz)->valor = valor;
			(*raiz)->izq = NULL;
			(*raiz)->der = NULL;
		} 
	}
	else {
		Arbol paux = *raiz;
		if (paux != NULL && valor < paux->valor != valor) {
			paux->izq = addRecursive(paux->izq, valor);
		}
		else if (paux != NULL && valor > paux->valor != valor) {
			paux->der = addRecursive(paux->der, valor);
		}
	}
}

// izdo > raiz > dcho
void inOrden(Arbol raiz){
	if (raiz != NULL) {
		inOrden(raiz->izq);
		printf("%02d ", raiz->valor);
		inOrden(raiz->der);
	}
}

// raiz > izdo > dcho
void preOrden(Arbol raiz) {
	if (raiz != NULL) {
		printf("%02d ", raiz->valor); // pre
		preOrden(raiz->izq);
		preOrden(raiz->der);
	}
}

// izdo > dcho > raiz
void postOrden(Arbol raiz) {
	if (raiz != NULL) {
		preOrden(raiz->izq);
		preOrden(raiz->der);
		printf("%02d ", raiz->valor); // post
	}
}
