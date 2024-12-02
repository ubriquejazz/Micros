#include <stdio.h>
#include "funciones.h"

int main() {
  int num1, num2, opcion, resultado = 0;

  opcion = gestionarOpcion();
  while (opcion != 6)  {
    calcularOperacion(opcion, &num1, &num2, &resultado);
    mostrarResultado(opcion, num1, num2, resultado);
    opcion = gestionarOpcion();
  }
  return 0;
}