
#ifndef APP_H__
#define APP__H__

#include <msp430.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "date.h"

#define dormido     _BIS_SR(LPM3_bits + GIE)
#define despierto   _BIC_SR_IRQ(LPM3_bits)
#define cargaTB     0x8000  //cuenta máxima del Timer B



#define OPTO_OUT P4OUT
#define OPTO_DIR P4DIR
#define OPTO_SEL P4SEL

#define RELE_OUT P5OUT
#define RELE_DIR P5DIR
#define RELE_SEL P5SEL

typedef enum {APP_STATE_INIT, APP_STATE_IDLE, APP_STATE_WAIT} APP_STATES;

typedef struct {
	APP_STATES state;

	bool    bat;                    // indica la situación de la batería.
	date_t   dia;
	int     dayofweek;
	int     minutos;                 //minutos utilizados para temporizador.
	int     horas;                   //horas utilizados para temporizador.

} APP_DATA;

extern APP_DATA appData;

void APP_Initialize (void);
void APP_Tasks (void);

#endif
