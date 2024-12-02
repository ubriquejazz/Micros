#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abc.h"

int colaVacia(Cola miCola){
  if(miCola == NULL)
    return 1;
  else
    return 0;
}

Arbol pop(Cola *miCola){
  Arbol nodo=NULL;
  if(*miCola == NULL)
    return NULL;
  else{
    nodo = (*miCola)->valor;
    Cola aux = *miCola;
    (*miCola)=(*miCola)->sig;
    free(aux);
  }
  return nodo;
}

void push(Cola *miCola, Arbol nodo)
{
  if(*miCola == NULL) {
    *miCola = (Cola)malloc(sizeof(tipoNodoCola));
    if (*miCola != NULL) {
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

void inserta_helper(Arbol *raiz, char nombre[], int edad) {
  Cola miCola = NULL;
  push(&miCola,(*raiz)->izq);
  push(&miCola,(*raiz)->der);

  Arbol nodo;
  while(colaVacia(miCola) == 0){
    nodo = pop(&miCola);
    if(nodo->izq == NULL){
      insertaNodo(&(nodo->izq), nombre, edad);
      break;
    }
    else if(nodo->der == NULL){
      insertaNodo(&(nodo->der), nombre, edad);
      break;
    }
    else { 
      push(&miCola, nodo->izq);
      push(&miCola, nodo->der);
    }
  }
}

void insertaNodo(Arbol *raiz, char nombre[], int edad) {
  if (*raiz == NULL) {
    *raiz = (Arbol)malloc(sizeof(tipoNodoArbol));
    if (*raiz != NULL) {
			strcpy((*raiz)->nombre,nombre);
			(*raiz)->edad = edad;
			(*raiz)->izq = NULL;
			(*raiz)->der = NULL;
		} 
  }
  else {
      if ((*raiz)->izq == NULL){
        insertaNodo(&((*raiz)->izq), nombre,edad);
      }
      else if ((*raiz)->der == NULL){
        insertaNodo(&((*raiz)->der), nombre,edad);
      }
      else { 
        Cola miCola = NULL;
        push(&miCola,(*raiz)->izq);
        push(&miCola,(*raiz)->der);
        Arbol nodo;
        while (colaVacia(miCola) == 0){
          nodo = pop(&miCola);
          if (nodo->izq == NULL){
            insertaNodo(&(nodo->izq), nombre, edad);
            break;
          }
          else if (nodo->der == NULL){
            insertaNodo(&(nodo->der), nombre, edad);
            break;
          }
          else { 
            push(&miCola, nodo->izq);
            push(&miCola, nodo->der);
          }
        }  
      }
  }
}

int edadMedia(Arbol raiz) {
  int cont=0, acum=0;
  if (raiz == NULL)
    return 0;
  Cola miCola = NULL;
  if (raiz->izq != NULL){
    push(&miCola, raiz->izq);
    push(&miCola, raiz->der);
  }
  Arbol nodo;
  while (colaVacia(miCola) == 0){
    nodo = pop(&miCola); // mujer
    if (nodo->izq != NULL){
      push(&miCola, nodo->izq);
      push(&miCola, nodo->der);
    }
    acum += nodo->edad;
    nodo = pop(&miCola); // hombre
    if (nodo->der != NULL){
      push(&miCola, nodo->izq);
      push(&miCola, nodo->der);
    }
    cont += 2;
    acum += nodo->edad;
  }
  return acum/cont;
}

int edadMediaHombres(Arbol raiz){
  int cont=0, acum=0;
  if(raiz == NULL)
    return 0;
  Cola miCola = NULL;
  if(raiz->izq != NULL){
    push(&miCola, raiz->izq);
    push(&miCola, raiz->der);
  }
  Arbol nodo;
  while(colaVacia(miCola) == 0){
    nodo = pop(&miCola); // mujer
    if(nodo->izq != NULL){
      push(&miCola, nodo->izq);
      push(&miCola, nodo->der);
    }
    nodo = pop(&miCola); // hombre
    if(nodo->der != NULL){
      push(&miCola, nodo->izq);
      push(&miCola, nodo->der);
    }
    cont++;
    acum = acum + nodo->edad;
  }
  return acum/cont;
}

int datosHombres(Arbol raiz) {
  if (raiz == NULL)
    return 0;
  Cola miCola = NULL;
  if (raiz->izq != NULL){
    push(&miCola, raiz->izq);
    push(&miCola, raiz->der);
  }
  Arbol nodo;
  while(colaVacia(miCola) == 0){
    nodo = pop(&miCola); // mujer
    if(nodo->izq != NULL){
      push(&miCola, nodo->izq);
      push(&miCola, nodo->der);
    }
    nodo = pop(&miCola); // hombre
    if(nodo->der != NULL){
      push(&miCola, nodo->izq);
      push(&miCola, nodo->der);
    }
    printf("\n%s tiene una edad de %d", nodo->nombre, nodo->edad);
  }
  return 1;
}

/* comienza el recorrido inorden del árbol */
void inOrden(Arbol raiz)
{
  if(raiz!=NULL){
    inOrden(raiz->izq);
    printf("%s ",raiz->nombre);
    inOrden(raiz->der);
  }
}

void preOrden(Arbol raiz)
{
  if(raiz!=NULL){
    printf("%s ",raiz->nombre);
    preOrden(raiz->izq);
    preOrden(raiz->der);
  }
}

void postOrden(Arbol raiz)
{
  if(raiz!=NULL){
    postOrden(raiz->izq);
    postOrden(raiz->der);
    printf("%s ",raiz->nombre);
  }
}