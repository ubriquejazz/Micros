/*
 * Date: 2022-05
 * File: lista telefonos (sesion 18)
 */

#include <stdio.h>
#include <string.h>
#include "funciones.h"

int main()
{
	Lista agenda = NULL;
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
	
	ListaTfnos_anadir(&agenda, &nElementos, p1);
	int pos = ListaTfnos_buscar(agenda, nElementos,"Fran", 0);
	printf("\nFran está en la posición %d",pos);
	ListaTfnos_imprimir(agenda);	
		
	ListaTfnos_anadir(&agenda, &nElementos, p2);
	pos = ListaTfnos_buscar(agenda,nElementos,"Fran", 0);
	printf("\nFran está en la posición %d",pos);
	ListaTfnos_imprimir(agenda);	
	
	ListaTfnos_anadir(&agenda, &nElementos, p3);
	pos = ListaTfnos_buscar(agenda,nElementos,"Fran", 0);
	printf("\nFran está en la posición %d",pos);
	ListaTfnos_imprimir(agenda);	
	
	ListaTfnos_anadir(&agenda, &nElementos, p4); 
	pos = ListaTfnos_buscar(agenda,nElementos,"Fran", 7);
	printf("\nFran está en la posición %d",pos);
	ListaTfnos_imprimir(agenda);	
	
	pos = ListaTfnos_buscar(agenda, nElementos,"Fran", 2);
	printf("\nFran está en la posición %d",pos);
	ListaTfnos_imprimir(agenda);	
	
	return 0;
}
