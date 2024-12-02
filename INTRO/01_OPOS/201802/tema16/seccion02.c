#include <stdio.h>
#include <string.h>
#include <pthread.h>

// returns the old value of a memory location and sets 
// the value to 1 as a single atomic operation.
boolean testandset(boolean* lock) { return *lock; }

// competencia por la variable cerrojo (viola mutex)
int busy;
void Cerrojo() {
    while(1) {
        while (busy == 1);
        busy = 1;
        printf("proceso en la sección crítica.\n");
        printf("variable busy debería estar en SC.");
        busy = 0;
    }
}

// RoundRobin : viola la propiedad de progreso
int turno;  // RR
int i = 0;  // RR
void RoundRobin() {
    while(1) {
        while (turno != i);
        printf("proceso en la sección crítica.\n");
        printf("un proceso que no está en espera de ejecutar su SC");
        printf("está bloqueando a otro proceso que quiere ejecutar su SC");
        turno = (i+1) % N;
    }
}

int main() {
    busy = 1;
    Cerrojo();   
    RoundRobin();

    // si un proceso esta evaluando testandset() 
    // ningun otro puede procesarlo en paralelo
    boolean lock = false;   // test, set and lock!
    while(1) {
        while (testandset(&lock));
        printf("proceso en la sección crítica.\n garantiza");
        printf("el acceso exclusivo pero no la espera limitada");
        lock = false;
    }
    return 0;
}