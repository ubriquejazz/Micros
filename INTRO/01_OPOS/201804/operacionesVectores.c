#include <stdio.h>
#define MAX 10

void insertarOrdenado(int nueva, int vector[], int *n){
  if((*n)>MAX){
    printf("Vector repleto");
    return;
  }
  for(int i=(*n);i>0;i--){
    if(vector[i-1]>nueva){
      vector[i]=vector[i-1]; 
    }
    else{
      vector[i]=nueva;
      break;
    }
  }
  if(vector[0]>nueva || (*n)==0){
    vector[0]=nueva;
  }
  
  (*n)++;
}

void insertarDetras(int nueva, int vector[], int *n){
  if((*n)>MAX){
    printf("Vector repleto");
    return;
  }
  vector[*n]=nueva;
  (*n)++;
}

void insertarDelante(int nueva, int vector[], int *n){
  if((*n)>MAX){
    printf("Vector repleto");
    return;
  }
  for(int i=(*n);i>0;i--){
    vector[i]=vector[i-1]; 
  }
  vector[0]=nueva;
  (*n)++;
}

void eliminarPrimero(int vector[], int *n){
	if(*n==0)
		return;
	for(int i=0; i<(*n)-1;i++)
		vector[i]=vector[i+1];
	(*n)--;
}

void eliminarUltimo(int vector[], int *n){
	if(*n==0)
		return;
	(*n)--;
}

void eliminar(int elemento, int vector[], int *n){
  int eliminado = 0;
  int pos;
  for(int i=0; i<(*n) && eliminado==0;i++){
    if(vector[i]==elemento){
      pos = i;
      eliminado = 1;
    }
  }
  if(eliminado == 1){
    for(int i=pos; i<(*n)-1;i++){
      vector[i]=vector[i+1];
    }
    (*n)--;
  }
  else{
    printf("Elemento no encontrado");
  }
}

void imprimirVector(int vector[], int n){
  for(int i=0; i<n; i++){
    printf("%d ",vector[i]);
  }
}

int main(void) {
  int edades[MAX];
  int numEdades=0;
  printf("\n\n\nPrimera inserción\n");
  insertarOrdenado(8,edades,&numEdades);
  imprimirVector(edades,numEdades);
  printf("\n\n\nSegunda inserción\n");
  insertarOrdenado(3,edades,&numEdades);
  imprimirVector(edades,numEdades);
  printf("\n\n\nTercera inserción\n");
  insertarOrdenado(9,edades,&numEdades);
  imprimirVector(edades,numEdades);
  printf("\n\n\nCuarta inserción\n");
  insertarOrdenado(4,edades,&numEdades);
  imprimirVector(edades,numEdades);
  printf("\n\n\nQuinta inserción (por detrás)\n");
  insertarDetras(1,edades,&numEdades);
  imprimirVector(edades,numEdades);
  printf("\n\n\nSexta inserción (por delante)\n");
  insertarDelante(10,edades,&numEdades);
  imprimirVector(edades,numEdades);
  printf("\n");
  eliminar(10,edades,&numEdades);
  printf("Elimino 10\n");
  imprimirVector(edades,numEdades);
  printf("\n");
  printf("Elimino 4\n");
  eliminar(4,edades,&numEdades);
  imprimirVector(edades,numEdades);
  printf("\n");
  printf("Elimino 3\n");
  eliminar(3,edades,&numEdades);
  imprimirVector(edades,numEdades);
  printf("\n");
  printf("Elimino el primero\n");
  eliminarPrimero(edades,&numEdades);
  imprimirVector(edades,numEdades);
  printf("\n");
  printf("Elimino el último\n");
  eliminarUltimo(edades,&numEdades);
  imprimirVector(edades,numEdades);
  return 0;
}
