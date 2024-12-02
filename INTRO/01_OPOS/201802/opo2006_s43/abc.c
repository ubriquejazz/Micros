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

void escribe(Arbol raiz, char* fichero) {
    FILE *f;       // Declaro un puntero al inicio del archivo
    f = fopen(fichero, "wb");
    if ( f == NULL) {
        printf("Error: No pudo abrirse el archivo para escritura.\n");
        return;
    }

    char continuar; // boolean
    Arbol nodo;
    do {
        printf("Datos de un nodo:\n");

		//Línea para escribir un registro completo en el archivo
        fwrite(&nodo, sizeof(tipoNodoArbol), 1, f);
        printf("¿Quieres insertar otro (S/N)?");
        scanf("%c",&continuar);
 		while (continuar!='S' && continuar!='N') {
            printf("\nError, repite: ");
            scanf("%c",&continuar);
	    }
	} while (continuar == 'S');
	fclose(f);
}

void lee(Arbol* raiz, char* fichero) {
    FILE *f;        //Abro el modo lectura (binario)
    f = fopen(fichero, "rb");
    if (f == NULL) {
        printf("Error: No pudo abrirse el archivo para lectura.\n");
        return;
    }

	printf("Datos cargado en el fichero:\n");
  	while(!feof(f)){
		Arbol nodo;
	}
    fclose(f);
}
