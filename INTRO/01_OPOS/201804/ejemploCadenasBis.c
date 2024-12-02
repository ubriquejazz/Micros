#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

	char nombre[50];
	int edad, cont;
	char continuar;
    	
	do{
	  	printf("Dime el nombre: ");
		fgets(nombre,50, stdin);
    		// Remover salto de línea
		if ((strlen(nombre) > 0) && (nombre[strlen(nombre) - 1] == '\n'))
		{
			nombre[strlen(nombre) - 1] = '\0';
		}
		printf("Dime la edad: ");
		scanf("%d",&edad);
		getchar();
		  
		printf("%s tiene %d años.\n",nombre,edad);
		printf("¿Quieres insertar otro (S/N)?");
		scanf("%c",&continuar);
		getchar();
	  	while (continuar!='S'&&continuar!='N') {
		  	printf("\nError, repite: ");
      		scanf("%c",&continuar);
      		getchar();
	  	}
	  	
  	} while (continuar == 'S');
  	
	return 1;
}
