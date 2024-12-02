/*
 * Date: 2022-04
 * File: simpross (sesion 15)
 */

#include "cliente.h"

void darAlta(Lista* lista, tipoCliente nuevoCliente)
{
	Lista nuevo;
	nuevo = (Lista)malloc(sizeof(tipoNodo));
	nuevo->valor = nuevoCliente;
	nuevo->sig=NULL;

	Lista aux=*lista;
	Lista detras=NULL;
	if (aux==NULL) {
		*lista=nuevo;
		return;
	}
	while (aux !=NULL &&) (aux->valor).horas < nuevoCliente.horas) {
        detras = aux;
		aux = aux->sig;
	}
	if (detras==NULL) {
		nuevo->sig = *lista;
		*lista = nuevo;
	}
    else {
		nuevo->sig = aux;
		detras->sig = nuevo;
	}
}

void darBaja(Lista *lista,char nombre[])
{
	Lista aux = *lista;
	Lista detras = NULL;
	
	if(aux==NULL)
		return;
	
	while(aux!=NULL){
		if (strcmp((aux->valor).nombre,nombre)!=0) {
			detras = aux;
			aux = aux->sig;
		}
        else {
			if (detras==NULL) {
				*lista = aux->sig;
			}
            else {
				detras->sig = aux->sig;
			}
			free(aux);
			return;
		}
	}
}

void listar1(Lista lista) 
{
	Lista aux = lista;
	while(aux!=NULL){
		printf("\n%s (%d horas).", (aux->valor).nombre, (aux->valor).horas);
		aux = aux->sig;
	}
}

void listar2(Lista lista,int num) 
{
	Lista aux = lista;
	while(aux!=NULL){
		if((aux->valor).horas >= num)
			printf("\n%s (%d horas).", (aux->valor).nombre, (aux->valor).horas);
		aux = aux->sig;
	}
}
