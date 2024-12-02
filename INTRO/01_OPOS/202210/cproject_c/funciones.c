/*
  File: funciones_linked.c
  Autor: Juan Gago
  Date: 22/10 (sesion 32)
*/
#include <stdio.h>
#include <string.h>
#include "funciones.h"

float leerFondos(){
	float h;
	scanf("%f",&h);
	while(h<1){
		printf("Mal, repite: ");
		scanf("%f",&h);
	}
	return h;
}

int leerUnidadGasto(){
	int h;
	scanf("%d",&h);
	while(h<1){
		printf("Mal, repite: ");
		scanf("%d",&h);
	}
	return h;
}

int leerVigencia(){
	int h;
	scanf("%d",&h);
	while(h!=0 && h!=1){
		printf("Mal, repite: ");
		scanf("%d",&h);
	}
	return h;
}

void darAlta (Lista*lista, int *cont, tipoValor nuevoValor){
	//if(buscarProyecto(*lista,nuevoValor->titulo)!=-1)
		//return;
	nuevoValor.cod=*cont;
	Lista nuevo;
	nuevo = (Lista)malloc(sizeof(tipoNodo));
	if(nuevo==NULL)
		return;
	nuevo->valor = nuevoValor;
	nuevo->sig=*lista;
	*lista=nuevo;
	(*cont)++;
}

void incrementarFondos (Lista lista, int cod, float cantidad){
	Lista aux=lista;
	
	if(aux==NULL)
		return;
	while(aux!=NULL){
		if((aux->valor).cod != cod){
			aux = aux->sig;
		}
		else{
			(aux->valor).fondos += cantidad;
			return;
		}
	}
}

void listar1(Lista lista, float limite){
	Lista aux = lista;
	while(aux!=NULL){
		if((aux->valor).fondos >= limite && (aux->valor).vigencia ==1){
			printf("\nCodigo: %d - Responsable: %s - Unidad gasto: %d.",(aux->valor).cod,(aux->valor).responsable,(aux->valor).unidadGasto);
		}
		aux = aux->sig;
	}
}

void listar2(Lista lista){
	ListaAux lista2=NULL;
	//recorro la lista de proyectos y voy insertando cada proyecto a la listaDpto (darAltaDep)
	while(lista!=NULL){
		darAltaDep(&lista2,(lista->valor).unidadGasto);
		lista=lista->sig;
	}
	
	//recorro la lista de dptos y muestros el campo unidad de gasto
	while(lista2!=NULL){
		printf("\nDepartamento: %d",lista2->valor);
		lista2 = lista2->sig;
	}
}

int buscarDepartamento(ListaAux listaDpto, int unidadGasto){
	while(listaDpto!=NULL){
		if((listaDpto->valor)==unidadGasto)
			return 1;
		else
			listaDpto = listaDpto->sig;
	}
}

void darAltaDep(ListaAux* listaDpto, int unidadGasto){
	if(buscarDepartamento(*listaDpto, unidadGasto)==0){
		ListaAux nuevo;
		nuevo = (ListaAux)malloc(sizeof(tipoNodoListaAux));
		if(nuevo==NULL)
			return;
		nuevo->valor = unidadGasto;
		nuevo->sig=*listaDpto;
		*listaDpto=nuevo;
	}
}
