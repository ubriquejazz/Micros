#include <stdio.h>

typedef enum {
	Entero, 
	Real, 
	Fraccion
} Clase_e;

typedef struct{
	int numerador;
	int denominador;
} Fraccion_t;

typedef union{
	int 		valorEntero;
	float 		valorReal;
	Fraccion_t 	fraccion;
} Valor_t;

typedef struct{
	Clase_e clase;
	Valor_t valor;
} tipoNumero;

typedef tipoNumero tipoNumeros[3];

int main(){
	tipoNumeros valores;
	valores[0].clase = Entero;
	if(valores[0].clase == Entero){
		valores[0].valor.valorEntero = 13;
	}
	else if(valores[0].clase == Real){
		valores[0].valor.valorReal = 13.00;
	}
	else{
		valores[0].valor.fraccion.numerador = 13;
		valores[0].valor.fraccion.denominador = 13;
	}
	printf("%d",valores[0].valor.valorEntero);
	return 0;
}