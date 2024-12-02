#include "profes.h"
#include <stdio.h>

void mostrarMenu() {
  printf("\nMenú de opciones:");
  printf("\n\tPulse 1 Dar de alta.");
  printf("\n\tPulse 2 Dar de baja.");
  printf("\n\tPulse 3 Lista ordenados por número de horas.");
  printf("\n\tPulse 4 Lista por un cierto número de horas.");
  printf("\n\tPulse 5 para salir.");
  printf("\n\tElija opion: ");
}

int leerOpcion() {
  int opcion;
  scanf("%d", &opcion);
  while (opcion < 1 || opcion > 5) {
    printf("Mal, repite: ");
    scanf("%d", &opcion);
  }
  return opcion;
}

int main() {
  Vector_t profesores;
  int numProfes = 0;
  int opcion, cont = 1;

  do {
    mostrarMenu();
    opcion = leerOpcion();
    switch (opcion) {
    case 1:
      darAlta(profesores, &numProfes, &cont);
      break;
    case 2:
      darBaja(profesores, &numProfes);
      break;
    case 3:
      ordenarProfes(profesores, numProfes);
      listadoProfes(profesores, numProfes);
      break;
    case 4:
      listadoProfesHoras(profesores, numProfes);
      break;
    default:
      printf("Bye");
      break;
    }
  } while (opcion != 5);
  return 0;
}
