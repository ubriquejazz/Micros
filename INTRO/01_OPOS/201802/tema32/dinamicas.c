#include <stdio.h>
#include <string.h>

// RESERVA DE MEMORIA
void test00() {
    char *cad;
    cad = malloc(250); 
    
    if (!cad) { // equivale a cad == NULL 
        printf("Error en la reserva");
        return;
    } 
    cad[0] = 'h';
    cad[249]= 'y';
}

// LIBERACIÓN DE MEMORIA
void test01() {
    char *cad[100]; 
    int i;

    for (i=0;i<100;i++) {
        printf("%d\n", i);
        if ((cad[i]=malloc(128))==NULL) { 
            printf("Error en la reserva");
            return;
        } 
    }

    /***** Operaciones con cad ********/

    for (i=0;i<100;i++) {
        free(cad[i]);
        //printf("%d\n", i);
    }
}

int main() {
    test01();
    return 0;
}