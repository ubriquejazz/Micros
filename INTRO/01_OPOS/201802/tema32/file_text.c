#include <stdio.h>
#include <string.h>
#include "file.h"

int main() {
    struct Persona persona;
    strcpy(persona.nombre, "Juan");
    persona.edad = 25;
    persona.altura = 1.75;

    // Escribir los datos en un textFile binario en modo texto
    FILE* textFile = fopen("datos.txt", "wt");
    if (textFile != NULL) {
        fprintf(textFile, "%s\n%d\n%.2f", persona.nombre, persona.edad, persona.altura);
        fclose(textFile);
    } else { return 1; }

    // Leer los datos desde el textFile binario en modo texto
    struct Persona personaLeida;
    textFile = fopen("datos.txt", "rt");
    if (textFile != NULL) {
        fscanf(textFile, "%s\n%d\n%f", personaLeida.nombre, &personaLeida.edad, &personaLeida.altura);
        fclose(textFile);
    } else { return 1; }

    return 0;
}
