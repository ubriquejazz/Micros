#include "io.h"

void IO_Init_Out(struct io out)
{
	out.libre     = 1;
	out.remoto    = 0;      //Por defecto modo LOCAL
	out.direccion = 1;
	return;
}

void IO_Init_Inp(struct io in)
{
	in.libre      = 1;
	in.remoto     = 0;
	in.direccion  = 1;
	return;
}

