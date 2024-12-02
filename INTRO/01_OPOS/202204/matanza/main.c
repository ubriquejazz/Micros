
#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"

void mostrarMenu(){
	printf("\nMenú de opciones:");
	printf("\n\tPulse 1 para dar de alta un nuevo cliente.");
	printf("\n\tPulse 2 para dar de baja un cliente.");
	printf("\n\tPulse 3 para listar los lientes que han asistido al menos a una matanza.");
	printf("\n\tPulse 4 para listar los clientes que han asistido a un número de matanzas determinado.");
	printf("\n\tPulse 5 para calcular la media de matanzas de todos los clientes.");
	printf("\n\tPulse 6 para salir.");
	printf("\n\tElija opion: ");
}

int leerOpcion(){
	int opcion;
	scanf("%d",&opcion);
	while(opcion<1 || opcion >6){
		printf("Mal, repite: ");
		scanf("%d",&opcion);
	}
	return opcion;
}

int leerTipoId(){
	int n;
	scanf("%d",&n);
	while(n<0 || n>1){
		printf("Error, repite: ");
		scanf("%d",&n);
	}
	return n;
}

int leerPositivo(){
	int n;
	scanf("%d",&n);
	while(n<=0){
		printf("Error, repite: ");
		scanf("%d",&n);
	}
	return n;
}

// tipoVector clientes;
Lista lista = NULL;	

int main()
{
	int numClientes=0;
	int opcion, nMatanzas;
	char nombre[30];
	float media;
	tipoCliente nuevo;
	
	do{
		mostrarMenu();
		opcion=leerOpcion();
		switch(opcion){
			case 1: 
				printf("Dame tu nombre: ");
				scanf("%s",nuevo.nombre);
				printf("Pulsa 0 si te identificar con DNI, 1 con pasaporte: ");
				nuevo.tipoId=leerTipoId();				
				if(nuevo.tipoId==0){
					printf("Dame tu DNI: ");
					scanf("%s",nuevo.id.dni);
				}
                else{
					printf("Dame tu pasaporte: ");
					scanf("%s",nuevo.id.pasaporte);
				}
				printf("Dime el pais: ");
				scanf("%s",nuevo.pais);
				printf("Dame el ultimo año de asistencia: ");
				nuevo.ultimoAnyo = leerPositivo();
				printf("Dame el número de matanzas que ha asistido: ");
				nuevo.numMatanzas = leerPositivo();
				printf("Dame el número de personas de la unidad familiar: ");
				nuevo.numPersonas = leerPositivo();
				printf("Dime tu correo: ");
				scanf("%s",nuevo.email);
				darAlta(&lista, nuevo);
				numClientes++;
				// darAlta(clientes, &numClientes, nuevo);
			    break;
			case 2:
				printf("Dame el nombre del cliente a eliminar: ");
				scanf("%s",nombre);
				darBaja(&lista, nombre);
				numClientes--;
			    // darBaja(clientes, &numClientes, nombre);
			    break;
			case 3: 
				listar1Matanza(lista);
				// listar1Matanza(clientes, numClientes);
			    break;
			case 4: 
				printf("Dame el número de matanzas que se establece como límite: ");
				nMatanzas = leerPositivo();
				// listarOrdenado(clientes, numClientes, nMatanzas);
				listarOrdenado(lista, nMatanzas);
				break;
			case 5:
				media = calcularMediaMatanzas(lista);
				// media = calcularMediaMatanzas(clientes, numClientes);
				printf("La media de matanzas es de %f.",media);
				break;
			default: printf("BYE BYE");
		}
		
	} while(opcion!=6);
	
	return 0;
}
