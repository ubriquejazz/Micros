/*
  File: binario.h
  Autor: Juan Gago
  Desc: Read and write a binary file
*/

#include <stdio.h>
#include <stdlib.h>
#include "binario.h"

int main()
{
    generarFichero("alumnos.txt");
    leerFichero("alumnos.txt");
}
