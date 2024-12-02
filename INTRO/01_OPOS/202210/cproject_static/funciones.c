/*
  File: funciones_static.c
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

void darAlta(tipoVector vector, int*numElementos, int*cont, tipoValor nuevoValor){
	//if(buscarProyecto(*lista,nuevoValor->titulo)!=-1)
		//return;
	if(*numElementos == N)
		return;
	nuevoValor.cod=*cont;
	vector[*numElementos]=nuevoValor;
	(*cont)++;
	(*numElementos)++;
}

void incrementarFondos(tipoVector vector, int numElementos, int cod, float cantidad){
	if(numElementos == 0)
		return;
	bool encontrado = false;
	for(int i=0; i<numElementos; i++){
		if(vector[i].cod==cod){
			vector[i].fondos += cantidad;
			encontrado = true;
			break;
		}
	}
}

void listar1(tipoVector vector, int numElementos, float limite){
	for(int i=0; i<numElementos; i++){
		if(vector[i].fondos >= limite && vector[i].vigencia == 1){
			printf("\nCodigo: %d - Responsable: %s - Unidad gasto: %d.",vector[i].cod,vector[i].responsable,vector[i].unidadGasto);
		}
	}
}

void listar2(Lista lista){
	ListaAux lista2=NULL;
	//recorro la lista de proyectos y voy insertando cada proyecto a la listaDpto (darAltaDep)
	for(int i=0;i<numElementos;i++){
		darAltaDep(&lista2, vector[i].unidadGasto);
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
