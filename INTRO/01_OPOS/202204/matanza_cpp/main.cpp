#include <iostream>
using namespace std;

const int MAX_CLI = 100;

struct cliente_t {
    string cliente;
    int tipoID;
    string pais;
    int ultimoAnio;
    string email;
};

typedef cliente_t vector_t[MAX_CLI];


int main()
{
    cout << "Marrano feliz" << endl;
}