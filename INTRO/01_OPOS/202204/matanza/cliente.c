/*
 * Date: 2022-04
 * File: marrano (sesion 14)
 */

#include <string.h>
#include "cliente.h"

/* 

void darAlta(tipoVector vector, int *numElementos, tipoCliente nuevo) {
    if (*numElementos == N)
        return;
    vector[*numElementos] = nuevo;
    (*numElementos)++;
}

void darBaja(tipoVector vector, int *numElementos, char nombre[]) {
    int pos = -1;
    if(*numElementos == 0)
        return;
    for(int i=0; i < (*numElementos) && pos == -1; i++) {
        if (strcmp(vector[i].nombre, nombre) == 0)
            pos = 1;
    }
    if (pos == -1)
        return; // no lo he encotrado
    for (int i = pos; i < (*numElementos); i++) {
        vector[i] = vector[i+1];
    }
    (*numElementos)--;
}

void ordenarVector(tipoVector vector, int numElementos) {
  tipoCliente aux;
  for (int i = 0; i < numElementos - 1; i++) {
	for (int j = i + 1; j < numElementos; j++) {
      		if (strcmp(vector[i].nombre, vector[j].nombre)>0) {
        		aux = vector[i];
        		vector[i]=vector[j];
        		vector[j]=aux;
      		}
    	}
  }
}

void listarNmatanza(tipoVector vector, int numElementos, int n) {
    ordenarVector(vector, numElementos);
    for(int i=0; i < numElementos; i++) {
        if (vector[i].numMatanzas == n)
            printf("\n%s: %d.", vector[i].nombre, vector[i].numMatanzas);
    }
}
 
void listar1matanza(tipoVector vector, int numElementos) {
    listarNmatanza(vector, numElementos, 1);
}

double calcularMediaMatanzas(tipoVector vector, int numElementos) {
	float media=0;
	for(int i=0;i<numElementos;i++)
		media = media + vector[i].numMatanzas;
	return media/(float)numElementos;
}

*/

void darAlta (Lista* lista, tipoCliente nuevoCliente){
	Lista nuevo;
	nuevo = (Lista)malloc(sizeof(tipoNodo));
	nuevo->valor = nuevoCliente;
	nuevo->sig=NULL;
	Lista aux=*lista;
	Lista detras=NULL;
	if(aux==NULL){
		*lista=nuevo;
		return;
	}
	while(aux !=NULL && strcmp((aux->valor).nombre,nuevoCliente.nombre) < 0) {
        detras=aux;
		aux = aux->sig;
	}
	if(detras==NULL){
		nuevo->sig = *lista;
		*lista=nuevo;
	}
	else {
		nuevo->sig=aux;
		detras->sig=nuevo;
	}
}

void darBaja(Lista *lista,char nombre[]){
	Lista aux = *lista;
	Lista detras = NULL;
	
	if(!aux) return;
	
	while(aux!=NULL){
		if(strcmp((aux->valor).nombre,nombre)!=0){
			detras = aux;
			aux = aux -> sig;
		}
		else{
			if(detras==NULL){
				*lista=aux->sig;
			}
			else{
				detras->sig=aux->sig;
			}
			free(aux);
			return;
		}
	}
}

void listar1Matanza(Lista lista){
	Lista aux = lista;
	while(aux!=NULL){
		if((aux->valor).numMatanzas>=1)
			printf("\n%s (%d matanzas).",(aux->valor).nombre,(aux->valor).numMatanzas);
		aux = aux->sig;
	}
}

void listarOrdenado(Lista lista,int nMatanzas){
	Lista aux = lista;
	while(aux!=NULL){
		if((aux->valor).numMatanzas>=nMatanzas)
			printf("\n%s (%d matanzas).",(aux->valor).nombre,(aux->valor).numMatanzas);
		aux = aux->sig;
	}
}

float calcularMediaMatanzas(Lista lista){
	float media=0;
	int cont=0;
	Lista aux = lista;
	while(aux!=NULL){
		media = media + (aux->valor).numMatanzas;
		cont++;
		aux = aux->sig;
	}
	return media/cont;
}

