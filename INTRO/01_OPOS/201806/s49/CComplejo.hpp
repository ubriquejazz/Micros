class CComplejo {
	private:
		double real, imag;
	public:
		CComplejo(); // constructor por defecto
		CComplejo(double r, double i); // constructor extendido
		~CComplejo();
		void asigna_real(double r);
		void asigna_imag(double i);
		double parte_real();
		double parte_imag();
		void suma(const CComplejo& a, const CComplejo& b);
		const void operator=(CComplejo& c);
		const void operator+(CComplejo& c);
		bool operator<(CComplejo &c);
};

void CComplejo::asigna_real(double r){
	real= r;
}

void CComplejo::asigna_imag(double i){
	imag = i;
}

double CComplejo::parte_imag(){
	return imag; // atributo imag del argumento implicito
}

double CComplejo::parte_real(){
	return real; // atributo imag del argumento implicito
}

void CComplejo::suma(const CComplejo& a, const CComplejo& b){
	real= a.real + b.real;
	imag= a.imag + b.imag;
}

CComplejo::CComplejo(){
	real= 1;
	imag= 0;
}

CComplejo::CComplejo(double r, double i){
	real= r;
	imag= i;
}

CComplejo::~CComplejo(){
// destruir el numero complejo...
}

const void CComplejo::operator=(CComplejo& c){
	real=c.parte_real();
}

const void CComplejo::operator+(CComplejo& c){
	real= real + c.parte_real();
	imag= imag + c.parte_real();
}

bool CComplejo::operator<(CComplejo &c){
	return this->parte_real() < c.parte_real();
}