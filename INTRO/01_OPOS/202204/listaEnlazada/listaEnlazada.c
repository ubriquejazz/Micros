/*
 * Date: 2022-04
 * File: listas (sesion 13)
 */

#include "listaEnlazada.h"

int SIZE(Lista lista){
    int num=0;
    pNodo aux = lista; 
    while(aux!=NULL){
        num++;
        aux = aux->sig;
    }
    return num;
}

void imprimirLista(Lista lista){
    pNodo aux = lista;
    while(aux != NULL){
        printf("%d ",aux->valor);
        aux = aux ->sig;
    }
}

void insertarDelante(Lista *lista, int v) {
    pNodo nuevo;
    nuevo = (pNodo)malloc(sizeof(tipoNodo));
    nuevo->valor = v;
    nuevo->sig = *lista;
    *lista = nuevo;
    return;
}

void insertarDetras(Lista *lista, int v){
    pNodo nuevo;
    nuevo = (pNodo)malloc(sizeof(tipoNodo));
    nuevo->valor = v;
    nuevo->sig = NULL;

    pNodo aux = *lista;
    if(aux == NULL){
        *lista = nuevo;
        return;
    }
    while(aux->sig!=NULL){
        aux = aux->sig;
    }
    aux->sig=nuevo;
    return;
}

void insertarOrdenado(Lista *lista, int v){
    pNodo nuevo;
    nuevo = (pNodo)malloc(sizeof(tipoNodo));    // new() in Java
    nuevo->valor = v;
    nuevo->sig = NULL;

    pNodo aux = *lista;     // recorrer la lista con dos punteros
    pNodo detras = NULL;    // 
    if(aux == NULL){
        *lista = nuevo;
        return;
    }
    while(aux != NULL && aux->valor < v){
        detras = aux;
        aux = aux->sig;
    }
    if(detras == NULL){
        nuevo->sig = *lista;
        *lista = nuevo;
    }
    else {
        nuevo->sig = aux;
        detras->sig = nuevo;
    }
    return;
}

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

void eliminar(Lista *lista, int v){
    pNodo aux = *lista;
    pNodo detras = NULL; 
    
    if(!aux)
        return;
    while (aux!=NULL && aux->valor!=v) {
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
    
