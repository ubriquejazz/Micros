#include<stdio.h>

#define N 20

void insertarNota(float notas[N],int*numNotas);
float calcularMedia(float notas[N],int numNotas);
float calcularMax(float notas[N],int numNotas);
float calcularMin(float notas[N],int numNotas);
int contarSiete(float notas[N],int numNotas);
void imprimir(float notas[N],int numNotas);

int main(){
	float notas[N];
	int numNotas=0;
	int opcion, media, max, min,cont;
	
	do{
		printf("\nOpciones:\n");
		printf("\n\tPulse 1 para insertar nota.");
		printf("\n\tPulse 2 para nota media.");
		printf("\n\tPulse 3 para nota máxima.");
		printf("\n\tPulse 4 para contar 7.");
		printf("\n\tPulse 5 para nota mínima.");
		printf("\n\tPulse 6 para salir.");
		
		scanf("%d",&opcion);
		while(opcion<1 || opcion>6){
			printf("Mal, repite: ");
			scanf("%d",&opcion);
		}
		switch(opcion){
			case 1:
				insertarNota(notas,&numNotas);
				imprimir(notas,numNotas);
				break;
			case 2:
				media=calcularMedia(notas,numNotas);
				printf("La nota media es de %d",media);
				break;
			case 3:
				max = calcularMax(notas,numNotas);
				printf("La nota máxima es de %d",max);
				break;
			case 4:
				cont = contarSiete(notas,numNotas);
				printf("El numero de 7 es de %d",cont);
				break;
			case 5:
				min = calcularMin(notas,numNotas);
				printf("La nota mínima es de %d",min);
				break;
			default: printf("ADIOS");
		}		
	}while(opcion!=6);	
	return 0;
}

void insertarNota(float notas[N],int*numNotas){
	float nota;
	printf("Dame la nota: ");
	scanf("%f",&nota);
	while(nota<0 || nota>10){
		printf("Mal, repite: ");
		scanf("%d",&nota);
	}
	notas[*numNotas]=nota;
	(*numNotas)++;
}

void imprimir(float notas[N],int numNotas){
	for(int i=0;i<numNotas;i++)
		printf("%f ",notas[i]);
	printf("\n");
}

float calcularMedia(float notas[N],int numNotas){
	float media=0;
	for(int i=0; i<numNotas; i++)
		media+=notas[i];
	media = media/numNotas;
	printf("---->%f",media);
	return media;
}

float calcularMax(float notas[N],int numNotas){
	float max = notas[0];
	for(int i=1; i<numNotas; i++){
		if(notas[i]>max)
			max = notas[i];
	}
	return max;
}

float calcularMin(float notas[N],int numNotas){
	float min = notas[0];
	for(int i=1; i<numNotas; i++){
		if(notas[i]<min)
			min = notas[i];
	}
	return min;
}

int contarSiete(float notas[N],int numNotas){
	int cont = 0;
	for(int i=0; i<numNotas; i++) {
		if(notas[i]==7)
			cont++;
	}
	return cont;
}