/*
 * Date: 2022-05
 * File: CPersona (sesion 19)
 */

#include <stdio.h>
#include <string.h>
#include "funciones.h"

int ListaTfnos_size(Lista lista){
    int num=0;
    pNodo aux = lista; 
    while(aux!=NULL){
        num++;
        aux = aux->sig;
    }
    return num;
}

void ListaTfnos_imprimir (Lista lista){
    pNodo aux = lista;
    while(aux != NULL){
        printf("\n%s %lu",aux->valor.nombre, aux->valor.telefono);
        aux = aux->sig;
    }
}

/** PRIVATE **/

void eliminarPrimero(Lista *lista){
    pNodo aux;
    aux=*lista;
    if(!aux)
        return;
    *lista = aux->sig;
    free(aux);
    return;
}

void eliminarUltimo(Lista *lista){
    pNodo aux;
    aux=*lista;
    pNodo detras = NULL; 
    if(!aux)
        return;

    while (aux->sig!=NULL){
        detras = aux;
        aux = aux->sig;
    }
    if(detras == NULL)
        *lista = aux->sig;
    else
        detras->sig = aux->sig;
    free(aux);
    return;
}

void insertarDelante(Lista *lista, Person_t v) {
    pNodo nuevo;
    nuevo = (pNodo)malloc(sizeof(tipoNodo));
    nuevo->valor = v;
    nuevo->sig = *lista;
    *lista = nuevo;
    return;
}

void insertarDetras(Lista *lista, Person_t v) {
    pNodo nuevo;
    nuevo = (pNodo)malloc(sizeof(tipoNodo));
    nuevo->valor = v;
    nuevo->sig = NULL;

    pNodo aux = *lista;
    if(aux == NULL){
        *lista = nuevo;
	// printf("..debugging..");
        return;
    }
    while(aux->sig!=NULL){
        aux = aux->sig;
    }
    aux->sig=nuevo;
    return;
}

/** PUBLIC **/

void ListaTfnos_anadir(Lista *lista, int *nEle, Person_t nuevo) {
    insertarDetras(lista, nuevo);
    (*nEle)++;
}

int ListaTfnos_buscar(Lista lista, int nEle, char nombre[50], int posInicio)
{
    int i = 0, pos = -1;
    pNodo aux = lista;
    if(posInicio >= nEle){
	return -1;
    }
    while(aux != NULL && i < posInicio) {
	i++;
	aux = aux->sig;
    }	
   
    while(aux != NULL && i < nEle){
	if(strcmp(aux->valor.nombre, nombre)==0){
		pos = i;
	} 
	i++;
	aux = aux->sig;
    }	
    return pos;
}
