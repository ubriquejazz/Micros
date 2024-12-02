#include <stdio.h>

#define N 20

void push(int pila[], int* cima, int nuevo);
void pop(int pila[], int* cima);
int top(int pila[], int cima);

void mostrar_menu(){
  	printf("\n\t++++Elige Opción:++++++");
  	printf( "\n\t1- Inicializar vector.");
  	printf( "\n\t2- Insertar elemento en vector.");
  	printf( "\n\t3- Sacar elemento pila.");
  	printf(" \n\t4- Tamaño de vector.");
  	printf("\n\t5-Cima del vector.");
  	printf("\n\t6- Salir");
  	printf("\n\t\tOPCION:\t");
}

int main() {
	int pila[N];
  	int cima;
  	int op;
  	int elemento;
  
   	do
    {
    	mostrar_menu();
        scanf("%d",&op);
       	while(op<1 || op>6) {
        	printf("Elige opción entre 1 y 5.");
           	scanf("%d",&op);
        }
       	switch (op){
         	case 1:		
         		cima=-1;
                break;
         	case 2:
         		printf ("\nIntroducir un elemento:");
      			scanf("%d",&elemento);
		 		push(pila, &cima, elemento);
                break;
         	case 3: 
                pop(pila,&cima);
                break;
         	case 4:
                printf("Size = %d. \n", cima+1);
                break;
         	case 5: 
			 	elemento = top(pila, cima);
                printf("La cima es %d. \n", elemento);
                break;
         	default: 
			 	printf("adios");  
       	}
    }while (op!=6);
    return 0;
}

void push(int pila[], int* cima, int nuevo) {
    if (*cima >= N - 1) {
        printf("\nOperación no realizada! La pila está llena.");
        return;
    }
    (*cima)++;
    pila[*cima] = nuevo;
    printf("\nHecho! Se ha introducido el elemento ->%d<- en la pila.", nuevo);
}

void pop(int pila[], int* cima) {
    if (*cima < 0) {
        printf("\nOperación no realizada! La pila está vacía.");
        return;
    }
	int eliminado = pila[(*cima)];
    (*cima)--;
    printf("\nHecho! Se ha extraído el elemento ->%d<- de la pila.", eliminado);
}

int top(int pila[], int cima) {
    if (cima < 0) {
        printf("\nOperación no realizada! La pila está vacía.\n");
        return -1;
    }
    return pila[cima];
}

