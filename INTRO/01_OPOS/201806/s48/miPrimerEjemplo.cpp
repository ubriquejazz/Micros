//Examen de 2018 PES INFORMATICA EN ANDALUCÍA

#include <iostream>

using namespace std;

void mostrarResultado(int op, float n1, float n2, float r);
void dividir(float n1, float n2,float& r);
float multiplicar(float n1, float n2);
void restar(float n1, float n2,float& r);
float sumar(float n1, float n2);
void calcularResultado(int op, float n1, float n2, float& resultado);
void leerDivision(float& n);
float leerNumero();
int mostrarMenu();

int main(){
	int op;
	float n1,n2, resultado;
	
	do{
	
		op=mostrarMenu();
	
		if(op != 5){
    		printf("Dame número 1: ");
    		n1 = leerNumero();
		
			printf("Dame número 2: ");
			if(op == 4){
				leerDivision(n2);
			}else{
				n2 = leerNumero();
			}
  		}
  	
  		calcularResultado(op,n1,n2,resultado);
  	
  		mostrarResultado(op,n1,n2,resultado);
  	}while(op!=5);
	return 0;
}

int mostrarMenu(){
	int opcion;
	do{
    	cout<< "Ejercicio de la calculadora"<<endl;
    	cout<<"1-Sumar"<<endl;
    	cout<<"2-Restar"<<endl;
    	cout<<"3-Multiplicar"<<endl;
    	cout<<"4-Dividir"<<endl;
    	cout<<"5-Salir"<<endl;
  
    	cin>>opcion;
  	}while(opcion<1 || opcion > 5);
  	return opcion;
}

void mostrarResultado(int op, float n1, float n2, float r){
	switch(op){
		case 1:
			cout<<n1<<" + "<<n2<<" = "<<r<<endl;
			break;
		case 2:
			cout<<n1<<" - "<<n2<<" = "<<r<<endl;
			break;
		case 3:
			cout<<n1<<" x "<<n2<<" = "<<r<<endl;
			break;
		case 4:
			cout<<n1<<" / "<<n2<<" = "<<r<<endl;
			break;
		default:
			cout<<"BYE BYE"<<endl;
	}
}
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
			cout<<"BYE BYE"<<endl;
	}
}


