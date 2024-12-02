main() {

float x,y; 
float *p1;  /* Declaración de p1 como puntero a float */ 
int *p2;    /* Declaración de p2 como puntero a entero */

p1=&x; /* Asigna a p1 la dirección de memoria de x */
/* Es correcto puesto que el puntero tiene el mismo tipo de base que la variable cuya dirección va a almacenar */

p2=&x; /* Asigna a p2 la dirección de memoria de y*/
/* NO es correcto puesto que el puntero NO tiene el mismo tipo de base que la variable cuya dirección va a almacenar. Sin embargo el error no va a ser detectado por el compilador */

y=*p2; /* NO va a tener el resultado esperado, puesto que la asignación anterior NO ha sido correcta*/

}