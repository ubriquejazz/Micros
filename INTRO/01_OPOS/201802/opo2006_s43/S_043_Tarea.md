## Oposiciones PES 2006 – Supuesto 4 (Programación C)

En un programa escrito en C, donde se manejan árboles, disponemos de la siguiente definición:

    typedef struct node {
        long n;
        struct node* izda;
        struct node* dcha;
    } NODO;

Implemente las funciones

    escribe(NODO* a, char* fic)
 
que escribe la información representada en el árbol 3 en el fichero cuyo nombre se encuentra 
indicado por la cadena **fic**, para poder recuperarlo posteriormente por la función “lee”.

    lee(NODO** a, char* fic)

que lee el contenido del fichero cuyo nombre se encuentra indicado por la cadena **fic**, y 
crea un árbol que se devuelve mediante el parámetro a.

NOTA: Los árboles no tienen estar balanceados ni ordenados. 
Ambas funciones deben ser complementarias, esto es, si guardamos un árbol con la función 
**escribe** y lo recuperamos posteriormente con la función **lee**, el árbol debe quedar 
como estaba el original, con las mismas relaciones padre-hijo entre nodos que tenía antes 
de ser guardado.
