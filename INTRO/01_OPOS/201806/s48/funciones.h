#ifndef FUNCIONES_H
#define FUNCIONES_H

#define N 20

/*
  File: funciones.h
  Autor: Juan Gago
*/
void dividir(float n1, float n2,float& r);
float multiplicar(float n1, float n2);
void restar(float n1, float n2,float& r);
float sumar(float n1, float n2);
void calcularResultado(int op, float n1, float n2, float& resultado);

#endif

void dividir(float n1, float n2,float& r){
	r=0;
	while(n1 >= n2){
		r++;
		n1=n1-n2;
	}
}

float multiplicar(float n1, float n2){
	float r=0;
	for(int i=0; i<n2;i++){
		r = r + n1;
	}
	return r;
}
void restar(float n1, float n2,float& r){
	r = n1 - n2;
}

float sumar(float n1, float n2){
	return n1 + n2;	
}

void calcularResultado(int op, float n1, float n2, float& resultado){
	switch(op){
		case 1:
			resultado = sumar(n1,n2);
			break;
		case 2:
			restar(n1,n2,resultado);
			break;
		case 3:
			resultado = multiplicar(n1,n2);
			break;
		case 4:
			dividir(n1,n2,resultado);
			break;
		default:
			break;
			// cout<<"BYE BYE"<<endl;
	}
}