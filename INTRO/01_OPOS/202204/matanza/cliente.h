#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
 * Date: 2022-04
 * File: marrano (sesion 13)
 */

#include <stdio.h>
#include <stdlib.h>

#define N 100
#define STATIC_IMPL

typedef union {
    char dni[9];
    char pasaporte[12];
} tipoUnion;

typedef struct Client_s {
    char        nombre[50];
    int         tipoId;
    tipoUnion   id;
    char        pais[30];
    int         ultimoAnyo;
    int         numMatanzas;
    int         numPersonas;
    char        email[30];
} tipoCliente;

typedef struct nodo {
    tipoCliente valor;
    struct nodo* sig;
}tipoNodo;

/*
    typedef tipoCliente tipoVector[N];
    void darAlta(tipoVector vector, int* numElementos, tipoCliente nuevo);
    void darBaja(tipoVector, int* numElementos, char[]);  
    void listar1matanza(tipoVector vector, int numElementos);
    void ordenarVector(tipoVector vector, int numElementos);
    void listarNmatanza(tipoVector vector, int numElementos, int n);
    double calcularMediaMatanzas(tipoVector vector, int numElementos);
*/    

typedef tipoNodo* Lista;
void darAlta(Lista* lista,tipoCliente nuevo);
void darBaja(Lista *lista, char nombre[]);
void listar1Matanza(Lista lista);
void listarOrdenado(Lista lista,int nMatanzas);
float calcularMediaMatanzas(Lista lista);


#endif

