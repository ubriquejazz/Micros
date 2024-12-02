#ifndef FUNCIONES_H
#define FUNCIONES_H

/*
  File: funciones.h
  Autor: Juan Gago
*/

int leer2();        // operandos "distinto de cero"
void leer(int *n);  // lee los operandos sin ningun filtro
void leeOpcion(int *op);

void mostrarMenu();
int  gestionarOpcion(); // muestra menu y lee opcion elegida

// Operaciones
void sumar(int n1, int n2, int *r);
void restar(int n1, int n2, int *r);
int  dividir(int n1, int n2);
int  multiplicar(int n1, int n2);
void intercambiar(int* a, int* b);

// gestiona que operacion se debe hacer y se encarga de ella
void calcularOperacion(int op, int *n1, int *n2, int *r);
void mostrarResultado(int op, int n1, int n2, int r);

#endif /* FUNCIONES_H */