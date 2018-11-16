#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define dormido   _BIS_SR(LPM3_bits + GIE)
#define despierto _BIC_SR_IRQ(LPM3_bits)

#define mes_impar ((date.fecha.mes==1)|(date.fecha.mes==3)|(date.fecha.mes==5)|(date.fecha.mes==7)|(date.fecha.mes==8)|(date.fecha.mes==10))
#define mes_par ((date.fecha.mes==4)|(date.fecha.mes==6)|(date.fecha.mes==9)|(date.fecha.mes==11))

//**************************  Tipos  *******************************************

struct hora{
  unsigned int seg;
  unsigned int min;
  unsigned int hr;
}; 

struct fecha{
  unsigned int dd;
  unsigned int mes;
  unsigned int aa;
}; 

struct date{
  struct hora hora;
  struct fecha fecha;
};  

struct programa{
  bool ud_on[6];
  bool ud_off[6];  
  struct hora hora_on;
  struct hora hora_off;
};

struct conf{
  bool on;                    //indica si filtrado, caudal o motor están activados
  unsigned int io;            //se registra la salida o entrada correspondiente.
};  

struct io{                    //registro que almacena las características de cada entrada o salida.
  unsigned int libre;         //libre = 0 ,, ocupada.
  unsigned int remoto;        //si remoto = 1 ,, salida controlada remotamente.
  unsigned int direccion;     //variable válida sólo si remoto = 1.
};

struct ud{
  bool on;
  bool desconex;
  struct hora hora_off;
  unsigned int remoto;        //si remoto = 1 ,, unidad de riego se controla remotamente.
  unsigned int direccion;     //variable válida sólo si remoto = 1.
  unsigned int sal;
};

struct alarma{
  bool on;
  unsigned int num_alarmas;   //Estos campos los actualizo cuando sensor Nivel avise de algún
  struct hora hora;           //problema con depósito o entrada del Filtro o Error bus.
  struct fecha fecha;
};  

void Configuracion(void);
struct hora Limpia_Hora(struct hora h);
struct fecha Limpia_Fecha(struct fecha f);
void Limpia_Programa(void);

void Menu(void);
void Linea(unsigned int linea);
void Ejecuta(unsigned int linea);

void Manual(void);
void Unidad(void);
void Motor(void);
void Estado_Dep(void);

void Programacion(void);
void Dias(void);
void marca(void);
void Veces(void);
void programas(void);
unsigned int activa_ud(unsigned int i, unsigned int texto);
void ud_onoff(unsigned int ud, unsigned int i, unsigned int texto);
void Litros(unsigned int i);

void Estado(void);
void pinta_al(unsigned int texto);
void Borra(void);
void Ver_al(unsigned int texto);

void Conf(void);
void Caudal(void);
void Pinta_Conf(unsigned int texto);

void Conf_IO(void);
void Zonas(void);
void Pinta_Zona(unsigned int ud);
unsigned int Local(unsigned int i, struct io inout[8], unsigned int ent_sal);
void pinta_inout(unsigned int num, unsigned int ent_sal);
void Remoto(void);
void Pinta_Ud(unsigned int ud);
void IN_OUT(unsigned int ent_sal_ocup, struct io in_out[8], unsigned int array_ocup[9]);
void pinta_io(unsigned int num, unsigned int ent_sal);
void Pinta_OutIn(unsigned int i, unsigned int ent_sal);
void Ver_Conf(void);
void ejecuta_ver(unsigned int sublinea);
void Pinta_reg(unsigned int i, struct io inout[8], unsigned int ent_sal);
void OnOff_Rele(void);

void Conf_AL(void);
void Pinta_Alarma(unsigned int texto);
void on_off(unsigned int texto, unsigned int conf_al_ud, unsigned int i, unsigned int sublinea);
void si_no(unsigned int sn, unsigned int texto, unsigned int conf_al_ud, unsigned int i, unsigned int sublinea);

void Reloj(void);
void Poner_Hora(unsigned int hora, unsigned int i, unsigned int ud);
void Poner_Fecha(void);
void actualiza_aa(void);
void actualiza_mm(unsigned int min_mes);
void Hora(int min, int hr);
void Fecha(unsigned int dd,
           unsigned int mes,
           unsigned int aa);

void Deposito(void);
void Porcentaje(unsigned int lleno_riego);

void Guarda_Conf(void);
void Write_Flash(void);
void Read_Flash(void);

unsigned long Numero(unsigned long maximo, unsigned int longitud, unsigned char posicion);
bool Iguales(struct hora hora1, struct hora hora2);

void Modo(void);
int Dia_Semana(void);
void test_boton(void);