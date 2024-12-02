/*
 * Date: 2022-05
 * File: lista telefonos (sesion 18)
 */

#include <stdio.h>
#include <string.h>

#define N 100

typedef struct{
	char nombre[50];
	char direccion[50];
	long telefono;
} Person_t;

typedef Person_t Vector_t[N];

int ListaTfnos_buscar(Vector_t v, int nEle, char nombre[50], int posInicio);
void ListaTfnos_anadir(Vector_t v, int *nEle, Person_t nuevo);

int main(){
	Vector_t agenda;
	int nElementos=0;
	
	Person_t p1;
	strcpy(p1.nombre, "Mila");
	strcpy(p1.direccion, "Calle de Alhama");
	p1.telefono = 111111111;
	
	Person_t p2;
	strcpy(p2.nombre, "Paco");
	strcpy(p2.direccion, "Calle de Huércal");
	p2.telefono = 222222222;
	
	Person_t p3;
	strcpy(p3.nombre, "Fran");
	strcpy(p3.direccion, "Calle de Almería");
	p3.telefono = 333333333;
	
	Person_t p4;
	strcpy(p4.nombre, "Bea");
	strcpy(p4.direccion, "Calle de Bea");
	p4.telefono = 444444444;
	
	ListaTfnos_anadir(agenda, &nElementos,p1);
	int pos = ListaTfnos_buscar(agenda,nElementos,"Fran",0);
	printf("\nFran está en la posición %d",pos);
			
	ListaTfnos_anadir(agenda, &nElementos,p2);
	pos = ListaTfnos_buscar(agenda,nElementos,"Fran",0);
	printf("\nFran está en la posición %d",pos);
	
	ListaTfnos_anadir(agenda, &nElementos,p3);
	pos = ListaTfnos_buscar(agenda,nElementos,"Fran",0);
	printf("\nFran está en la posición %d",pos);
	
	ListaTfnos_anadir(agenda, &nElementos,p4);
	pos = ListaTfnos_buscar(agenda,nElementos,"Fran",7);
	printf("\nFran está en la posición %d",pos);
	
	pos = ListaTfnos_buscar(agenda,nElementos,"Fran",2);
	printf("\nFran está en la posición %d",pos);
	
	return 0;
}

void ListaTfnos_anadir(Vector_t v, int *nEle, Person_t nuevo){
	v[*nEle]=nuevo;
	(*nEle)++;
}
	
int ListaTfnos_buscar(	Vector_t v, int nEle, 
						char nombre[50], int posInicio){
	int pos = -1;
	if(posInicio>= nEle){
		return -1;
	}
	for(int i=posInicio; i<nEle && pos == -1; i++){
		if(strcmp(v[i].nombre,nombre)==0){
			pos = i;
		}
	}
	return pos;
}
