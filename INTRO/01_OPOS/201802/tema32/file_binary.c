#include <stdio.h>
#include <string.h>
#include "file.h"

int main() {
    struct Persona persona;
    strcpy(persona.nombre, "Juan");
    persona.edad = 25;
    persona.altura = 1.75;

    // Escribir los datos en un binFile binario
    FILE* binFile = fopen("datos.bin", "wb");
    if (binFile != NULL) {
        fwrite(&persona, sizeof(struct Persona), 1, binFile);
        fclose(binFile);
    } else { return 1; }

    // Leer los datos desde el binFile binario
    struct Persona personaLeida;
    binFile = fopen("datos.bin", "rb");
    if (binFile != NULL) {
        fread(&personaLeida, sizeof(struct Persona), 1, binFile);
        fclose(binFile);
    } else { return 1; }

    return 0;
}
