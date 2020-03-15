#ifndef FUNCTIONS__H__
#define FUNCTIONS__H__

#include <msp430.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define tmpo_pantalla_ppal 40

extern struct io input[], out[];

void Configuracion(void);
void Write_Flash(void);
void Read_Flash(void);

#endif
