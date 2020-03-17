#ifndef MENU__H__
#define MENU__H__

#include <msp430.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "libreria.h"

extern bool         flanco;
extern unsigned int modo, ON;

void Linea(unsigned int linea);
void Ejecuta(unsigned int linea);
void Menu(void);

#endif
