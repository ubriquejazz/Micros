#include <stdio.h>
#include "profes.h"

int leerTipoId() {
	int opcion;
    printf("\nIntroduce el tipo de identificación (0-DNI | 1-Pasaporte):");
	scanf("%d",&opcion);
	while(opcion<0 || opcion >1){
		printf("Mal, repite: ");
		scanf("%d",&opcion);
	}
	return opcion;
}

int leerHoras() {
	int opcion;
    printf("\nIntroduce el número de horas: ");
	scanf("%d",&opcion);
	while(opcion<0 || opcion >100){
		printf("Mal, repite: ");
		scanf("%d",&opcion);
	}
	return opcion;
}

int leerDia() {
	int opcion;
    printf("\nIntroduce el número de días al año (entre 1 y 365): ");
	scanf("%d",&opcion);
	while(opcion<1 || opcion >30){
		printf("Mal, repite: ");
		scanf("%d",&opcion);
	}
	return opcion;
}

// ver insertarOrdenado
void darAlta (Vector_t profesores, int* numProfes, int* cont)
{
    Profe_t nuevo;
    int insertado = 0;
    if (*numProfes == NPROFES)
        printf("Vector lleno.");
    else 
    {
        nuevo.cod = *cont;

        nuevo.tipoId = leerTipoId();
        if (nuevo.tipoId == DNI) {
            printf("\n\t Dame tu DNI: ");
            scanf("%s", nuevo.id.dni);
        }
        else {
            printf("\n\t Dame tu passaporte: ");
            scanf("%s", nuevo.id.pasaporte);
        }

        printf("\n\t Introduce nombre:");
        scanf("%s", nuevo.nombre);

        nuevo.nHoras = leerHoras();
        nuevo.diaComienzo = leerDia();

        for (int i=*numProfes; i>0; i--) {
            if (profesores[i-1].nHoras < nuevo.nHoras)
                profesores[i] = profesores[i-1];
            else {
                profesores[i] = nuevo;
                insertado = 1;
            }   
        }
        if (!insertado) {
            profesores[0] = nuevo;
        }
            
        (*numProfes)++;
        (*cont)++;
    }
}

void darBaja (Vector_t profesores, int* numProfes)
{
    int codBaja, pos=-1;
    int encontrado = 0;

    printf("Dime el número del profe a eliminar: ");
    scanf("%d",&codBaja);
    for(int i=0; i < *numProfes && encontrado==0; i++){
        if(profesores[i].cod == codBaja){
            pos = i;
            encontrado = 1;
        }
    }
    if (!encontrado) 
        printf("Elemento no encontrado");
    else {
        for(int i=pos; i < *numProfes - 1; i++) {
            profesores[i]=profesores[i+1];
        }
        (*numProfes)--;
    }
}

void mostrarProfesor(Profe_t profesor) {
    printf("\n");
    printf("Cód(%d)", profesor.cod);
    
    if (profesor.tipoId == DNI) {
        printf("\tDNI: %s", profesor.id.dni);
    } 
    else {
        printf("\tPasaporte: %s", profesor.id.pasaporte);
    }
    printf("\tNombre: %s", profesor.nombre);
    printf("\tHoras: %d", profesor.nHoras);
    printf("\tDía: %d", profesor.diaComienzo);
}

// Listar los profesores 
void listadoProfes(Vector_t profesores, int numProfes)
{
    for(int i=0; i<numProfes; i++){
        mostrarProfesor(profesores[i]);
    }
}

// Ordna los profesores por número de horas de más a menos horas (método de burbuja)
void ordenarProfes (Vector_t profesores, int numProfes) 
{
    int cont=0;
    Profe_t temp;
    for(int i=0; i<(numProfes-1); i++) {
        for(int j=i; j<numProfes; j++) {
            if (profesores[j].nHoras > profesores[i].nHoras) { 
                temp = profesores[j]; 
                profesores[j] = profesores[i]; 
                profesores[i] = temp; 
                cont++;
            }
        }
    }
    printf("\nProfes cambiados: %d", cont);
}

// Profesores que dan un cierto número de horas, que se introducen por teclado
void listadoProfesHoras (Vector_t profesores, int numProfes) 
{
    int horas, pos = -1;
    printf("Dame las horas: ");
    horas = leerHoras();
    for(int i=0; i<numProfes; i++){
        if (profesores[i].nHoras == horas) {
            mostrarProfesor(profesores[i]); 
            pos = i;
        }
    }
    if (pos == -1) 
        printf("\nProfe no encontrado");
    else
        printf("\nProfe en la posicion: %d", pos);
}
