#include <stdio.h>

typedef char Cadena_t[20];

typedef enum{
	DNI,
	NIE,
	PASAPORTE,
	LICENCIA
}Forma_e;

typedef union{
	Cadena_t dni;
	Cadena_t nie;
	Cadena_t pasaporte;
	Cadena_t licencia;
}tipoId;

typedef struct{
	Cadena_t 	nombre;
	Forma_e  	formaId;
	tipoId 		id;
	int 		edad;
	float 		peso;
}tipoPersona;

int main()
{
	tipoPersona tu;
	printf("Dime tu nombre: ");
	scanf("%s",tu.nombre);
	printf("¿Cómo te identificas (0-DNI,1-NIE,2-PASAPORTE,3-LICENCIA)?: ");
	int opcion;
	scanf("%d",&tu.formaId);
	switch(tu.formaId){
		case DNI:
			printf("Dime tu dni: ");
			scanf("%s",tu.id.dni);
			break;
		case NIE:
			printf("Dime tu nie: ");
			scanf("%s",tu.id.nie);
			break;
		case PASAPORTE:
			printf("Dime tu pasaporte: ");
			scanf("%s",tu.id.pasaporte);
			break;
		default:
			printf("Dime tu licencia: ");
			scanf("%s",tu.id.licencia);
	}
	printf("Hola me llamo %s",tu.nombre);
	switch(tu.formaId){
		case DNI:
			printf(" y me identificao con el DNI numero %s: .",tu.id.dni);
			break;
		case NIE:
			printf(" y me identificao con el NIE numero %s: .",tu.id.nie);
			break;
		case PASAPORTE:
			printf(" y me identificao con el PASAPORTE numero %s: .",tu.id.pasaporte);
			break;
		default:
			printf(" y me identificao con la LICENCIA numero %s: .",tu.id.licencia);
	}
	return 0;
}