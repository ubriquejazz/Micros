#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int flag[2];
int turn;
int counter = 0;

void *proceso0(void *threadID) {
    while (counter < 10) {
        flag[0] = 1;
        turn = 1;
        while (flag[1] == 1 && turn == 1);

        // Sección crítica
        printf("P0 en la sección crítica.\n");
        printf("Valor del contador: %d\n", ++counter);
        flag[0] = 0;
    }
    pthread_exit(NULL);
}

void *proceso1(void *threadID) {
    while (counter < 10) {
        flag[1] = 1;
        turn = 0;
        while (flag[0] == 1 && turn == 0);

        // Sección crítica
        printf("*** P1 en la sección crítica.\n");
        printf("*** Valor del contador: %d\n", ++counter);
        flag[1] = 0;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[2];

    flag[0] = 0;
    flag[1] = 0;

    // Crear el hilo y ejecutar el proceso
    pthread_create(&threads[0], NULL, proceso0, NULL);
    pthread_create(&threads[1], NULL, proceso1, NULL);
  
    // Esperar a que el hilo termine
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    return 0;
}



