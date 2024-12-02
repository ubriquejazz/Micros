#include <stdio.h>
#include <string.h>

// ESCRITURA Y LECTURA DE CARACTERES
void test00() {
    int c; 
    while ( (c=getchar()) != '\n') 
        putchar (tolower(c));
}

// ESCRITURA Y LECTURA DE CADENAS
void test01() { 
    char frase[80];     // cadena de 80 caracteres 
    puts("Introduzca una línea de texto"); 
    gets(frase); 
    puts ("la frase introducida es: "); 
    puts(frase); 
}

// ESCRITURA Y LECTURA CON FORMATO
void test02() {
    int x; 
    char p; 
    printf("Introduzca un número: "); 
    scanf("%d", &x); /* lee un entero y lo almacena en x */ 

    printf ("\nIntroduzca una letra: ");
    scanf("%c", &p); /* lee un carácter y lo almacena en p */ 
    fflush(stdin);

    printf ("\nLos caracteres leídos son : %d y %c", x, p);
}



int main() {
    test00();
    return 0;
}