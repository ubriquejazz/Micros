#include<stdio.h>
#include "funciones.h"

int main(){
	int opcion, nota;
	do{
		mostrarMenu();
		opcion = leerOpcion();
		switch(opcion){
			case 1:
				printf("Dame la nota: ");
				nota = leerNota();
				insertarNota(nota);
				break;
			case 2:
				printf("La nota media es de %d",calcularMedia());
				break;
			case 3:
				printf("La nota máxima es de %d",maximaNota());
				break;
			case 4:
				printf("El numero de 7 es de %d",cuentaItems(7));
				break;
			case 5:
				printf("La nota mínima es de %d",minimaNota());
				break;
			default: printf("ADIOS");
		}		
		
	}while(opcion!=6);	
	return 0;
}