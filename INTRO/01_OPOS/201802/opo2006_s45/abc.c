#include <stdio.h>
#include <stdlib.h>
#include "abc.h"

void procesa_nodo_esc(NODO *a, FILE *f){
  if(a==NULL){
    fprintf(f, "%ld ", (long)0);
  }else{
    fprintf(f, "%ld ",a->n);    // preorden
    procesa_nodo_esc(a->izq, f);
    procesa_nodo_esc(a->der, f);
  }
}

void procesa_nodo_lec(NODO **a, FILE *f){
  long num;
  fscanf(f,"%ld", &num);
  if(num == 0){
    *a = NULL;
  }else{
    *a = (NODO *)malloc(sizeof(NODO));
    (*a)->n=num;
    procesa_nodo_lec(&((*a)->izq),f);
    procesa_nodo_lec(&((*a)->der),f);
  }
}

int escribe(NODO *a, char *fi){
  FILE *f;
  f = fopen(fi,"w");
  if(f == NULL)
    return 1;
  else{
    procesa_nodo_esc(a,f);
    fclose(f);
    return 0;
  }
}

int lee(NODO **a,char *fi){
  FILE *f;
  f = fopen(fi, "r");
  if(f == NULL)
    return 1;
  else {
    procesa_nodo_lec(a, f);
    fclose(f);
    return 0;
  }
}

void insertaNodo(NODO **a, long valor) {
	/* si el árbol está vacío */
	if (*a == NULL) {
		*a = (NODO *)malloc(sizeof(NODO));
		
    /* si la memoria está asignada, entonces asigna el valor */
		if (*a != NULL) {
			(*a)->n = valor;
			(*a)->izq = NULL;
			(*a)->der = NULL;
		} else {
			printf("No se insertó %ld.\n", valor);
		}
	} else {
		/* el valor a insertar es menor que el valor en el nodo actual */
		if (valor < (*a)->n) {
			insertaNodo(&((*a)->izq), valor);
		} else if (valor > (*a)->n){
			insertaNodo(&((*a)->der), valor);
		} else {
			printf(" duplicado");
		}
	}
}

void inOrden(NODO *a) {
	/* si el árbol no está vacío, entonces recórrelo */
	if (a != NULL) {
		inOrden(a->izq);
		printf("%3ld", a->n);
		inOrden(a->der);
	}
}