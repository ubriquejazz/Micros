#include <stdio.h>
#include "funciones.h"

// globales privadas
static float notas[N];
static int numNotas=0;

void mostrarMenu(){
    printf("\nOpciones:\n");
    printf("\n\tPulse 1 para insertar nota.");
    printf("\n\tPulse 2 para nota media.");
    printf("\n\tPulse 3 para nota máxima.");
    printf("\n\tPulse 4 para contar 7.");
    printf("\n\tPulse 5 para nota mínima.");
    printf("\n\tPulse 6 para salir.");
}

int leerOpcion(){
  int op;
  scanf("%d",&op);
  while(op<1 || op>6){
    printf("Mal, repite: ");
    scanf("%d",&op);
  }
  return op;
}

int leerNota(){
  int nota;
  scanf("%d",&nota);
  while(nota<0 || nota>10){
    printf("Mal, repite: ");
    scanf("%d",&nota);
  }
  return nota;
}

void insertarNota(int nota) {
  notas[numNotas]=nota;
  numNotas++;
}

int calcularMedia() {
  int media=0;
  for(int i=0; i<numNotas; i++)
    media+=notas[i];
  return media = media/numNotas;  
}

int maximaNota() {
  int max = notas[0];
  for(int i=1; i<numNotas; i++){
    if(notas[i]>max)
      max = notas[i];
  }
  return max;
}

int minimaNota() {
  int min = notas[0];
  for(int i=1; i<numNotas; i++){
    if(notas[i]<min)
      min = notas[i];
  }
  return min;
}

int cuentaItems(int nota) {
  int cont = 0;
  for(int i=0; i<numNotas; i++) {
    if(notas[i]==nota)
      cont++;
  }
  return cont;
}
