#include <stdio.h>
#include <stdlib.h>
#include "cola.h"

int SIZE(Cola miCola) {
	int num=0;
    pNodo aux = miCola; 
    while(aux!=NULL){
        num++;
        aux = aux->sig;
    }
    return num;
}

// Ojo! no hacer pop de una cola vacia
int empty(Cola miCola) {
	if(miCola == NULL)
		return 1;
	else
		return 0;
}

// extraer
int pop(Cola *miCola) {
	int nodo = (*miCola)->value;
    Cola aux = *miCola;
    (*miCola)=(*miCola)->sig;
    free(aux);
	return nodo;
}

// insertar
void push(Cola* miCola, int value) {
	if(*miCola == NULL) {
		*miCola = (Cola)malloc(sizeof(tipoNodo));
		if (*miCola != NULL) {
			(*miCola)->value = value;
			(*miCola)->sig = NULL;
		}
	}
	else{
		Cola aux = *miCola;
		Cola nodoCola;
		while(aux->sig != NULL){
			aux=aux->sig;
		}
		nodoCola = (Cola)malloc(sizeof(tipoNodo));
		nodoCola->value = value;
		nodoCola->sig = NULL;
		aux->sig = nodoCola;
	}
}