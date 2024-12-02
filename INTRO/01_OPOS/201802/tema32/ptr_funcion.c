#include <stdio.h>
#include <string.h>

// Definición de las funciones
int suma(int a, int b) {
    return a + b;
}

int resta(int a, int b) {
    return a - b;
}

// Declaración del tipo de puntero a función
typedef int (*OperacionMatematica)(int, int);
OperacionMatematica ptrOperacion = null;

// Función que utiliza un puntero a función como parámetro
void realizarOperacion(OperacionMatematica operacion, int a, int b) {
    int resultado = operacion(a, b);
    printf("El resultado es: %d\n", resultado);
}

int main() {
    int a = 10;
    int b = 5;

    ptrOperacion = suma;
    realizarOperacion(ptrOperacion, a, b);

    ptrOperacion = resta;
    realizarOperacion(ptrOperacion, a, b);

    return 0;
}
