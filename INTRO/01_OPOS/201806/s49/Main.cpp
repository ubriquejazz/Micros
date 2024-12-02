#include "CComplejo.hpp"
#include <iostream>
using namespace std;

int main()
{
	CComplejo a, s; // inicializados a 1+0i por el constructor
	CComplejo b(3,2);

	s.suma(a,b);
	cout << s.parte_real() << ", " // imprime 2, 0i
	<< s.parte_imag() << "i" << endl;
	
	a = b;
	cout << a.parte_real() << ", " // imprime 2, 0i
	<< a.parte_imag() << "i" << endl;
	
	cout << b.parte_real() << ", " // imprime 2, 0i
	<< b.parte_imag() << "i" << endl;
	
	a+b;
	cout << a.parte_real() << ", " // imprime 2, 0i
	<< a.parte_imag() << "i" << endl;
	
	bool resp = b<a;
	cout << resp << endl;
}