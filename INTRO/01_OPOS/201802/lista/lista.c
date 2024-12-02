#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

int SIZE(Lista lista){
    int num=0;
    pNodo aux = lista; 
    while(aux!=NULL){
        num++;
        aux = aux->sig;
    }
    return num;
}

void imprimir(Lista lista){
    pNodo aux = lista;
    while(aux != NULL){
        printf("%d ",aux->valor);
        aux = aux ->sig;
    }
}

void insertar(Lista* lista, int v){
    pNodo nuevo;
    nuevo = (pNodo)malloc(sizeof(tipoNodo));    // new() in Java
    nuevo->valor = v;
    nuevo->sig = NULL;

    pNodo aux = *lista;     // recorrer la lista con dos punteros
    pNodo detras = NULL;    // aux y detras
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

void eliminar(Lista* lista, int v){
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
    
