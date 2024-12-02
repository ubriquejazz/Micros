#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
  File: funciones.h
  Autor: Juan Gago
*/

typedef struct nodo{
  long n;
  struct nodo *izq;
  struct nodo *der;
} NODO;

typedef NODO* tipoNodo;

// f. que escribe la información del árbol 3 en el fichero cuyo nombre se encuentra 
// indicado por la cadena **fic**, 
int escribe(NODO *a, char *fi);

// para poder recuperarlo posteriormente por la función “lee”.
// lee el contenido del fichero cuyo nombre se encuentra indicado por la cadena **fic**, y 
// crea un árbol que se devuelve mediante el parámetro a.
int lee(NODO **a, char *fi);

void insertaNodo(NODO **a, long valor);

void inOrden(NODO *a);

#endif