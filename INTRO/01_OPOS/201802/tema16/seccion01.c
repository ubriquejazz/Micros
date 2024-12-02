#include <stdio.h>
#include <string.h>

// solucion hardware: el OS no puede cargar ningun proceso
// proceso0j en memoria (inadecuado para arq. multiprocesador)
void proceso0i() {
    while(1) {
        __disable_interrupts();
        printf("proceso en la sección crítica.\n");
        __enable_interrupts();
    }
}

// en general
void proceso0j() {
    while(1) {
        // seccion para usar recurso Rj
        // codigo que no usa recurso
        // ...
    }
}

int main() {
    proceso0i();
    proceso0j();
    return 0;
}