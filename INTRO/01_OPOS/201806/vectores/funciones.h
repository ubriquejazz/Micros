#ifndef FUNCIONES_H
#define FUNCIONES_H

#define N 20
/*
  File: funciones.h
  Autor: Juan Gago
*/

void mostrarMenu();
int leerOpcion();
int leerNota();

void insertarNota(int nota);
int calcularMedia();
int maximaNota();
int minimaNota();
int cuentaItems(int nota);

#endif /* FUNCIONES_H */