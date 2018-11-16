/****************************************************************************************/
/*																						*/
/*					  	LECTOR/PROGRAMADOR CHIP-CARD SLE4442 de 2Kbit 					*/
/*																						*/
/*				FICHERO: Expendedor.C       			FECHA: Junio 2002 				*/
/*				AUTOR:   Juan Manuel Gago Benítez		PFC 2001/2002 					*/
/*																						*/
/****************************************************************************************/

#include <utility.h>
#include <userint.h>
#include <formatio.h>
#include <ansi_c.h>
#include <rs232.h>
#include "expendedor.h"

/*********** Declaración de Constantes usadas por el Lector/Programador *****************/

char 	STX	= 2;
char	ETX	= 3;
char	ACK = 6;

char	RESET 	= 'A';
char	RDMMN	= 'C';
char	WRMM	= 'E';
char	RDSM	= 'H';
char	CMPSC	= 'J';
char	WRPSC	= 'K';
char	BUSY 	= 'U';
char	READY 	= 'V';
char	STATO	= 'S';

/***************************** Declaración de Variables Globales ************************/

int		com, caratula;
int 	MasterPIN;
char	ans[134];

/****************************************************************************************/
/* 																						*/	
/*				  	Subrutinas de Conversión Ascii-Hexadecimal-Decimal					*/
/*																						*/
/****************************************************************************************/

int Ascii2Int (char asc1, char asc2)
{
	int sal;
	sal = 16 * (asc1 - 48) + asc2 - 48;
	if ((sal > 99) | (sal < 0))	sal = 99;
	return sal;
}

char Ascii2Char (char asc1, char asc2)
{
	return 16 * (asc1 - 48) + (asc2 - 48);
}

void Int2Ascii(int i, char asc[3])
{
	asc[0] =  i/16 + 48;
	if (i%16 == 10) 	asc[1] = ':';
	if (i%16 == 11) 	asc[1] = ';';
	if (i%16 == 12) 	asc[1] = '<';
	if (i%16 == 13) 	asc[1] = '=';
	if (i%16 == 14) 	asc[1] = '>';
	if (i%16 == 15) 	asc[1] = '?';
	else				asc[1] = i%16 + 48; 
	asc[2] = 0;
}

void Char2Ascii(char c, char asc[3])
{
	asc[0] =  c/16 + 48;
	if (c%16 == 10) 	asc[1] = ':';
	if (c%16 == 11) 	asc[1] = ';';
	if (c%16 == 12) 	asc[1] = '<';
	if (c%16 == 13) 	asc[1] = '=';
	if (c%16 == 14) 	asc[1] = '>';
	if (c%16 == 15) 	asc[1] = '?';
	else				asc[1] = c%16 + 48; 
	asc[2] = 0;
}

void pin2PIN(int pin, char PIN[7])
{
	PIN[0] = '0';
	PIN[1] = (pin/100) + 48;
	PIN[2] = '0';
	PIN[3] = ((pin/10) % 10) + 48;
	PIN[4] = '0';
	PIN[5] = (pin%10) + 48;
	PIN[6] = 0;
}

/************************************ MostrarValidez() **********************************/
/*																						*/	
/* 		Muestra en los indicadores 'Desde' y 'Hasta' las fechas de inicio y final, 		*/
/*  	respectivamente, de la validez de la tarjeta insertada.							*/
/*																						*/
/*		Variable de Entrada: ans[134]													*/
/*																						*/
/****************************************************************************************/

void MostrarValidez()
{
	char	MM[3], AA[3], visita[20];

    /* Muestra fecha de inicio */
    
    Fmt(visita, "%i", Ascii2Int(ans[1],ans[2]));
    if (strlen (visita) < 2 )	{
    	visita[1] = visita[0];
    	visita[0] = '0';
    }
    visita[2] = 0;
	strcat (visita, " / ");

    Fmt(MM, "%i", Ascii2Int(ans[3],ans[4]));
    if (strlen (MM) < 2 )	{
    	MM[1] = MM[0];
    	MM[0] = '0';
    }
    MM[2] = 0;
	strcat (visita, MM);
	strcat (visita, " / ");

    Fmt(AA, "%i", Ascii2Int(ans[5],ans[6]));
    if (strlen (AA) < 2 )	{
    	AA[1] = AA[0];
    	AA[0] = '0';
    }
    AA[2] = 0;
	strcat (visita, AA);
	SetCtrlVal (caratula, PANEL_Desde, visita);
	
    /* Muestra fecha final */
    
    Fmt(visita, "%i", Ascii2Int(ans[7],ans[8]));
    if (strlen (visita) < 2 )	{
    	visita[1] = visita[0];
    	visita[0] = '0';
    }
    visita[2] = 0;
	strcat (visita, " / ");

    Fmt(MM, "%i", Ascii2Int(ans[9],ans[10]));
    if (strlen (MM) < 2 )	{
    	MM[1] = MM[0];
    	MM[0] = '0';
    }
    MM[2] = 0;
	strcat (visita, MM);
	strcat (visita, " / ");

    Fmt(AA, "%i", Ascii2Int(ans[11],ans[12]));
    if (strlen (AA) < 2 )	{
    	AA[1] = AA[0];
    	AA[0] = '0';
    }
    AA[2] = 0;
	strcat (visita, AA);
	SetCtrlVal (caratula, PANEL_Hasta, visita);
}

/************************************ MostrarVisitas() **********************************/
/*																						*/	
/* 		Muestra en la caja de texto 'Visitas' las fechas en que el propietario 			*/
/* 		de la tarjeta insertada entró al recinto controlado por nuestro sistema.		*/
/*																						*/
/*		Variable de Entrada: ans[134]													*/
/*																						*/
/****************************************************************************************/

void MostrarVisitas()
{
	char	DD[3], MM[3], AA[3], visita[20];
	int		i,longitud;

	/* Calculo el número de visitas realizadas */ 

    longitud = 1;													
    while ((ans[longitud] != '?') & (longitud < 61)) longitud = longitud + 6;

    /* Muestra las visitas realizadas */  

    for (i=1; i<longitud; i=i+6) {
    
    	Fmt(visita, "%i.- %c", (i-1)/6,0);
    	
    	Fmt(DD, "%i", Ascii2Int(ans[i],ans[i+1]));
    	if (strlen (DD) < 2 )	{
    		DD[1] = DD[0];
    		DD[0] = '0';
    	}
    	DD[2] = 0;
		strcat (visita, DD);
		strcat (visita, " / ");

    	Fmt(MM, "%i", Ascii2Int(ans[i+2],ans[i+3]));
    	if (strlen (MM) < 2 )	{
    		MM[1] = MM[0];
    		MM[0] = '0';
    	}
    	MM[2] = 0;
		strcat (visita, MM);
		strcat (visita, " / ");

    	Fmt(AA, "%i", Ascii2Int(ans[i+4],ans[i+5]));
    	if (strlen (AA) < 2 )	{
    		AA[1] = AA[0];
    		AA[0] = '0';
    	}
    	AA[2] = 0;
		strcat (visita, "20");
		strcat (visita, AA);

		InsertTextBoxLine (caratula, PANEL_Visitas, (i-1)/6, visita);
    }

	/* Representa las visitas que quedan mediante asteriscos */ 
	
    for (i=longitud; i<61; i=i+6) {										
    	Fmt(visita, "%s<%i%s", (i-1)/6,".- ** / ** / ****");
		InsertTextBoxLine (caratula, PANEL_Visitas, (i-1)/6, visita);
    }
}

/********************************* MostrarDatosUsuario() ********************************/
/*																						*/	
/* 			Muestra los datos personales del propietario de a tarjeta insertada.		*/
/*			Estos son necesarios para poder devolvérsela en caso de pérdida.			*/
/*																						*/
/*			Variable de Entrada: ans[134]												*/
/*																						*/
/****************************************************************************************/

void MostrarDatosUsuario()
{
	int 	i, j;    
    char	nombre[27], direccion[27], poblacion[15];
   
    i = 1; j = 0;
    while (i < 53) {
    	nombre[j] = Ascii2Char (ans[i], ans[i+1]); 		/*  Nombre  */
    	i = i + 2;
    	j++;
    }
    nombre[26] = 0;
    
    j = 0;												
    while (i < 105) {
    	direccion[j] = Ascii2Char (ans[i], ans[i+1]);	/* Dirección */
    	i = i + 2;
    	j++;
    }
    direccion[26] = 0;
    
    j = 0;
    while (i < 133) {
    	poblacion[j] = Ascii2Char (ans[i], ans[i+1]);	/* Población */    
    	i = i + 2;
    	j++;
    }
    poblacion[14] = 0;
    
	SetCtrlVal (caratula, PANEL_Usuario,   nombre);
	SetCtrlVal (caratula, PANEL_Direccion, direccion);
	SetCtrlVal (caratula, PANEL_Poblacion, poblacion);
}

/************************************* Inhabilita() *************************************/
/*																						*/	
/*		Cada vez que se pulse 'Nueva Tarjeta' se deshabilitan el menu de la derecha		*/
/*																						*/
/****************************************************************************************/

void Inhabilita()
{
	/* Inhabilita controles de escritura */
	
	SetCtrlAttribute (caratula, PANEL_Recargar, 		ATTR_DIMMED, 1);
	SetCtrlAttribute (caratula, PANEL_CambiarPIN, 		ATTR_DIMMED, 1);
	SetCtrlAttribute (caratula, PANEL_CambiarDATOS, 	ATTR_DIMMED, 1);
	SetCtrlAttribute (caratula, PANEL_CambiarVALIDEZ, 	ATTR_DIMMED, 1);
	
 	/*Inhabilita controles para introducir PIN */
 	
 	SetCtrlAttribute (caratula, PANEL_TarjetaVirgen,	ATTR_DIMMED, 1);
 	SetCtrlAttribute (caratula, PANEL_AceptarPIN,		ATTR_DIMMED, 1);
 	SetCtrlAttribute (caratula, PANEL_PIN, 				ATTR_DIMMED, 1);
 	SetCtrlAttribute (caratula, PANEL_Tiene,			ATTR_DIMMED, 1);
 	SetCtrlAttribute (caratula, PANEL_EC, 				ATTR_DIMMED, 1);
 	SetCtrlAttribute (caratula, PANEL_Intentos,			ATTR_DIMMED, 1);
}

/************************************ NuevaTarjeta() ************************************/
/*																						*/	
/*	  Comprueba si la tarjeta insertada es de nuestra aplicación y si lo es lee toda  	*/	
/*	  la información que tiene, mostrándola en los indicadores centrales del panel.		*/
/*																						*/
/****************************************************************************************/

int NuevaTarjeta()
{
	int		ec;
	char	comando[20];


	Inhabilita();
	
	/* Enciende LED rojo */

	Fmt (comando,"%s<%c%c%c",STX,BUSY,ETX);		
	ComWrt (com, comando, 3);
	ans[0] = ComRdByte (com);
	if  (rs232err < 0){ 
		SetCtrlVal (caratula, PANEL_Estado, 0);
		return 0;
	}
	
	/* Si no hay tarjeta, "Tarjeta no insertada" */
	
	Fmt (comando, "%s<%c%c%c",STX,STATO,ETX);
	ComWrt (com, comando, 3);
	ComRd  (com, ans, 3);
	if  (rs232err < 0){ 
		SetCtrlVal (caratula, PANEL_Estado, 0);
		return 0;
	}
	if 	(ans[1] == '0') { 
		SetCtrlVal (caratula, PANEL_Estado, 1);
		return 0;
	}
	
	/* Si Respuesta al Reset != A2131091, "Tarjeta errónea" */ 
	
	Fmt (comando,"%s<%c%c%c",STX,RESET,ETX);	
	ComWrt (com, comando, 3);
	ComRd  (com, ans, 10);
	if ((ans[1] != ':') & (ans[2] != '2') &
		(ans[3] != '1') & (ans[4] != '3') &
		(ans[5] != '1') & (ans[6] != '0') &
    	(ans[7] != '9') & (ans[8] != '1')) {
		SetCtrlVal (caratula, PANEL_Estado, 2);
		return 0;
	}
	
	/* Lectura de 6 bytes a partir del 32 (período de validez) */

	Fmt (comando,"%s<%c%c%c%c%c%c%c",STX,RDMMN,'2','0','0','6',ETX);
	ComWrt (com, comando, 7);
	ComRd  (com, ans, 14);
	MostrarValidez();

	/* Lectura de 30 bytes a partir del 38 (historial de entradas) */

	Fmt (comando,"%s<%c%c%c%c%c%c%c",STX,RDMMN,'2','6','1','>',ETX);
	ComWrt (com, comando, 7);
	ComRd  (com, ans, 62);
	MostrarVisitas();
	
	/* Lectura 66 bytes a partir del 68	(datos del usuario) */
	
	Fmt (comando,"%s<%c%c%c%c%c%c%c",STX,RDMMN,'4','4','4','2',ETX);
	ComWrt (com, comando, 7);											
	ComRd  (com, ans, 134);
	MostrarDatosUsuario();    

	/* Lectura del Contador de Errores. Si es 0, "Tarjeta bloqueada" */ 
	
	Fmt (comando,"%s<%c%c%c",STX,RDSM,ETX);
	ComWrt (com, comando, 3);
	ComRd  (com, ans, 10);
	ec = Ascii2Int (ans[1], ans[2]);
	if (ec == 4) ec = 5;
	SetCtrlVal (caratula, PANEL_EC, ec-4);
	if (ec == 0) {
		SetCtrlVal (caratula, PANEL_Estado, 5);
		return 0;
	}
	
	/* Si no, habilita controles para introducir PIN */
	
 	SetCtrlAttribute (caratula, PANEL_TarjetaVirgen,	ATTR_DIMMED, 0);
 	SetCtrlAttribute (caratula, PANEL_AceptarPIN,		ATTR_DIMMED, 0);
 	SetCtrlAttribute (caratula, PANEL_PIN, 				ATTR_DIMMED, 0);
 	SetCtrlAttribute (caratula, PANEL_Tiene,			ATTR_DIMMED, 0);
 	SetCtrlAttribute (caratula, PANEL_EC, 				ATTR_DIMMED, 0);
 	SetCtrlAttribute (caratula, PANEL_Intentos,			ATTR_DIMMED, 0);

	/* pero todavía "Escritura inhabilitada" */
	
	SetCtrlVal (caratula, PANEL_Estado, 3);
	
	return 0;
}

/************************************* PIN_correcto *************************************/
/*																						*/	
/*		Una vez introducido el PIN correcto, habilitamos los controles de escritura.	*/
/*																						*/
/****************************************************************************************/

void PIN_correcto()
{
	/* Habilita controles de escritura */
	
	SetCtrlAttribute (caratula, PANEL_Recargar, 		ATTR_DIMMED, 0);
	SetCtrlAttribute (caratula, PANEL_CambiarPIN, 		ATTR_DIMMED, 0);
	SetCtrlAttribute (caratula, PANEL_CambiarDATOS, 	ATTR_DIMMED, 0);
	SetCtrlAttribute (caratula, PANEL_CambiarVALIDEZ, 	ATTR_DIMMED, 0);
	
 	/*Inhabilita controles para introducir PIN */
 	
 	SetCtrlAttribute (caratula, PANEL_TarjetaVirgen,	ATTR_DIMMED, 1);
 	SetCtrlAttribute (caratula, PANEL_AceptarPIN,		ATTR_DIMMED, 1);
 	SetCtrlAttribute (caratula, PANEL_PIN, 				ATTR_DIMMED, 1);
 	SetCtrlAttribute (caratula, PANEL_Tiene,			ATTR_DIMMED, 1);
 	SetCtrlAttribute (caratula, PANEL_EC, 				ATTR_DIMMED, 1);
 	SetCtrlAttribute (caratula, PANEL_Intentos,			ATTR_DIMMED, 1);

	/* "Escritura habilitada" */
	
	MessagePopup ("Control de Tarjetas", "PIN correcto. Pulse OK para continuar...");
	SetCtrlVal (caratula, PANEL_Estado, 4);
}

/************************************* AceptarPIN() *************************************/
/*																						*/	
/*	  Comprueba si la clave tecleada en el control 'PIN' coincide con el  PSC de la		*/
/*	  tarjeta introducida. Si no es así decrementa el indicador 'EC'.				 	*/
/*																						*/
/****************************************************************************************/

int AceptarPIN()
{
	int		ec;
	int		pin;
	int		Unidades, Decenas, Centenas;

	char	PIN[7];	
	char	comando[20];

	/* Lectura del Contador de Errores. Si es 0, "Tarjeta bloqueada" */ 
	
	Fmt (comando,"%s<%c%c%c",STX,RDSM,ETX);
	ComWrt (com, comando, 3);
	ComRd  (com, ans, 10);
	ec = Ascii2Int (ans[1], ans[2]);
	if (ec == 4) ec = 5;
	SetCtrlVal (caratula, PANEL_EC, ec-4);
	if (ec == 0) {
		SetCtrlVal (caratula, PANEL_Estado, 5);
		return 0;
	}

	/* Cifra el PIN introducido con el PIN maestro */
	
	GetCtrlVal(caratula, PANEL_PIN, &pin);
	
	Unidades = (  pin%10      +  MasterPIN%10     ) % 10;
	Decenas  = ( (pin/10)%10  + (MasterPIN/10)%10 ) % 10;
	Centenas = ( (pin/10)/10  + (MasterPIN/10)/10 ) % 10;

	pin = Centenas * 100 + Decenas * 10 + Unidades;
	
	/* Compara el PIN cifrado con el PSC de la tarjeta */

	pin2PIN(pin,PIN);
	Fmt(comando,"%s<%c%c%s%c",STX,CMPSC,PIN,ETX);
	ComWrt (com, comando, 9);
	ans[0] = ComRdByte (com);
	if  (rs232err < 0){ 
		SetCtrlVal (caratula, PANEL_Estado, 0);
		return 0;
	}

	/* Lectura del Contador de Errores. Si es 0, "Tarjeta bloqueada" */ 
	
	Fmt (comando,"%s<%c%c%c",STX,RDSM,ETX);
	ComWrt (com, comando, 3);
	ComRd  (com, ans, 10);
	ec = Ascii2Int (ans[1], ans[2]);
	if (ec == 4) ec = 5;
	SetCtrlVal (caratula, PANEL_EC, ec-4);
	if (ec == 0) {
		SetCtrlVal (caratula, PANEL_Estado, 5);
		return 0;
	}
	
	/* Si no, el PIN es el correcto, enciende LED verde y llama a PIN_correcto() */	

	if (ec == 7) {
		Fmt(comando,"%s<%c%c%c",STX,READY,ETX);
    	ComWrt (com, comando, 3);
		ans[0] = ComRdByte (com);
		PIN_correcto();
	}
	
	return 0;
}

/********************************** TarjetaVirgen() *************************************/
/*																						*/	
/*	  Comprueba si el PSC genérico de una tarjeta virgen (FF FF FF hex) coincide con	*/
/*	  el  PSC de la tarjeta introducida. Si no es así decrementa el indicador 'EC'	 	*/
/*																						*/
/****************************************************************************************/

int TarjetaVirgen()
{
	int		ec;
	char	comando[20];

	/* Lectura del Contador de Errores. Si es 0, "Tarjeta bloqueada" */ 
	
	Fmt (comando,"%s<%c%c%c",STX,RDSM,ETX);
	ComWrt (com, comando, 3);
	ComRd  (com, ans, 10);
	ec = Ascii2Int (ans[1], ans[2]);
	if (ec == 4) ec = 5;
	SetCtrlVal (caratula, PANEL_EC, ec-4);
	if (ec == 0) {
		SetCtrlVal (caratula, PANEL_Estado, 5);
		return 0;
	}
				
	/* Compara el PIN 0xFF + 0xFF + 0xFF con el de la tarjeta */
	
	Fmt(comando,"%s<%c%c%s%c",STX,CMPSC,"FFFFFF",ETX);
	ComWrt (com, comando, 9);
	ans[0] = ComRdByte (com);
	if  (rs232err < 0){ 
		SetCtrlVal (caratula, PANEL_Estado, 0);
		return 0;
	}

	/* Lectura del Contador de Errores. Si es 0, "Tarjeta bloqueada" */ 
	
	Fmt (comando,"%s<%c%c%c",STX,RDSM,ETX);
	ComWrt (com, comando, 3);
	ComRd  (com, ans, 10);
	ec = Ascii2Int (ans[1], ans[2]);
	if (ec == 4) ec = 5;
	SetCtrlVal (caratula, PANEL_EC, ec-4);
	if (ec == 0) {
		SetCtrlVal (caratula, PANEL_Estado, 5);
		return 0;
	}
				
	/* Si no, el PIN es el correcto, enciende LED verde y llama a PIN_correcto() */	

	if (ec == 7) {
		Fmt(comando,"%s<%c%c%c",STX,READY,ETX);
	    ComWrt (com, comando, 3);
		ans[0] = ComRdByte (com);
		PIN_correcto();
	}

	return 0;
}


/*************************************** Recargar ***************************************/
/*																						*/	
/*						Recarga el saldo de la tarjeta introducida						*/
/*																						*/
/****************************************************************************************/

int Recargar()
{
	int		i;
	char	dir[3], comando[20];

	/* Escritura de 30 bytes a partir del 38 con el dato 0xFF */ 
	
	for (i=38; i<68; i++){
		Int2Ascii(i,dir);
		Fmt(comando, "%c%c%s%c%c%c",STX, WRMM, dir, '?', '?', ETX);
   		ComWrt (com, comando, 7);
		ans[0] = ComRdByte (com);
		if  (rs232err < 0){ 
			SetCtrlVal (caratula, PANEL_Estado, 0);
			return 0;
		}
	}

	MessagePopup ("Control de Tarjetas", "Tarjeta recargada. Pulse OK para continuar...");
	
	/* Lectura de 30 bytes a partir del 38 (historial de entradas) */  
	
	Fmt (comando,"%s<%c%c%c%c%c%c%c",STX,RDMMN,'2','6','1','>',ETX);
	ComWrt (com, comando, 7);
	ComRd  (com, ans, 62);
	MostrarVisitas();
	
	return 0;
} 

/*********************************** CambiarDATOS() *************************************/
/*																						*/	
/*   			Actualiza los datos del propietario de la tarjeta introducida			*/
/*																						*/
/****************************************************************************************/

int CambiarDATOS()
{
	int		i;
	char	comando[20];
	char	dir[3], letra[3];
	char	nombre[27], direccion[27], poblacion[15];

	/* Escritura del nombre del usuario */

	PromptPopup ("Control de Tarjetas", "Nombre", nombre, 26);

	for (i=strlen (nombre); i<26; i++)   nombre[i] = ' ';		/* nombre[i] = ' ' */

    for (i=0; i<26; i++){
       	Int2Ascii  (i+68, 	   dir);
		Char2Ascii (nombre[i], letra);
		Fmt(comando, "%c%c%s%s%c",STX, WRMM, dir, letra, ETX);
		ComWrt (com, comando, 7);
		ans[0] = ComRdByte (com);
		if  (rs232err < 0){ 
			SetCtrlVal (caratula, PANEL_Estado, 0);
			return 0;
		}
	}

	/* Escritura de la dirección del usuario */

    PromptPopup ("Control de Tarjetas", "Dirección", direccion, 26);

	for (i=strlen (direccion); i<26; i++)   direccion[i] = ' ';	/* direccion[i] = ' ' */
	
    for (i=0; i<26; i++){
       	Int2Ascii  (i+68+26, 	  dir);
		Char2Ascii (direccion[i], letra);
		Fmt(comando, "%c%c%s%s%c",STX, WRMM, dir, letra, ETX);
		ComWrt (com, comando, 7);
		ans[0] = ComRdByte (com);
		if  (rs232err < 0){ 
			SetCtrlVal (caratula, PANEL_Estado, 0);
			return 0;
		}
	}

	/* Escritura de la población del usuario */

	PromptPopup ("Control de Tarjetas", "Población", poblacion, 14);
	
    for (i=strlen (poblacion); i<14; i++)   poblacion[i] = ' ';	/* poblacion[i] = ' ' */
    
    for (i=0; i<14; i++){
      	Int2Ascii  (i+68+52, 	  dir);
		Char2Ascii (poblacion[i], letra);
		Fmt(comando, "%c%c%s%s%c",STX, WRMM, dir, letra, ETX);
		ComWrt (com, comando, 7);
		ans[0] = ComRdByte (com);
		if  (rs232err < 0){ 
			SetCtrlVal (caratula, PANEL_Estado, 0);
			return 0;
		}
	}

	/* Lectura de 66 bytes a partir del 68 (datos de usuario) */
	
	Fmt(comando,"%s<%c%c%c%c%c%c%c",STX,RDMMN,'4','4','4','2',ETX);	
	ComWrt (com, comando, 7);				
	ComRd (com, ans, 134);
	MostrarDatosUsuario();
	
	return 0;
} 

/************************************ CambiarPIN() **************************************/
/*																						*/	
/*						Actualiza el PIN de la tarjeta introducida						*/
/*																						*/
/****************************************************************************************/

int CambiarPIN()
{
	int		i;
	int		pin;
	int		Unidades, Decenas, Centenas;

	char	PIN[7];	
	char 	nuevopin[4];
	char	comando[20];

	/* Escanea el teclado hasta que se introduzcan un número menor de 4 dígitos */

	do 
		PromptPopup ("Control de Tarjetas", "Nuevo PIN", nuevopin, 3);
		
	while  (!isdigit(nuevopin[0]) | !isdigit(nuevopin[1]) | !isdigit(nuevopin[2]));
	
	for (i=strlen(nuevopin); i<4; i++)	nuevopin[i] = 0;
	
	/* Cifra el nuevo PIN introducido con el PIN maestro */

	Fmt (&pin, "%i<%s", nuevopin);
	
	Unidades = (  pin%10      +  MasterPIN%10     ) % 10;
	Decenas  = ( (pin/10)%10  + (MasterPIN/10)%10 ) % 10;
	Centenas = ( (pin/10)/10  + (MasterPIN/10)/10 ) % 10;

	pin = Centenas * 100 + Decenas * 10 + Unidades;

	/*   Escritura del nuevo PIN	*/
	
	pin2PIN(pin,PIN);
	Fmt(comando,"%s<%c%c%s%c",STX,WRPSC,PIN,ETX);	
	ComWrt (com, comando, 9);
	ans[0] = ComRdByte (com);
	if  (rs232err < 0){ 
		SetCtrlVal (caratula, PANEL_Estado, 0);
		return 0;
	}
	
	return 0;
} 

/********************************* CambiarVALIDEZ() *************************************/
/*																						*/	
/*	   	    	Actualiza el período de validez de la tarjeta introducida				*/
/*																						*/
/****************************************************************************************/

int CambiarVALIDEZ()
{
	int		i;
	char	fecha[9];
	char	comando[20];
	char	dir[3], letra[3];

    /* Fecha de Inicio */

	PromptPopup ("VALIDEZ", "Fecha de Inicio (dd/mm/aa)", fecha, 8);

	i = 10 * (fecha[0] - 48) +  fecha[1] - 48;
	Int2Ascii (i,  letra);
	Int2Ascii (32, dir);
	Fmt(comando, "%c%c%s%s%c",STX, WRMM, dir, letra, ETX);
	ComWrt (com, comando, 7);
	ans[0] = ComRdByte (com);
	if  (rs232err < 0){ 
		SetCtrlVal (caratula, PANEL_Estado, 0);
		return 0;
	}
	
   	i = 10 * (fecha[3] - 48) +  fecha[4] - 48;
   	Int2Ascii (i,  letra);
   	Int2Ascii (33, dir);
	Fmt(comando, "%c%c%s%s%c",STX, WRMM, dir, letra, ETX);
	ComWrt (com, comando, 7);
	ans[0] = ComRdByte (com);
	if  (rs232err < 0){ 
		SetCtrlVal (caratula, PANEL_Estado, 0);
		return 0;
	}

	i = 10 * (fecha[6] - 48) +  fecha[7] - 48;
	Int2Ascii (i,  letra);
	Int2Ascii (34, dir);
	Fmt(comando, "%c%c%s%s%c",STX, WRMM, dir, letra, ETX);
	ComWrt (com, comando, 7);
	ans[0] = ComRdByte (com);
	if  (rs232err < 0){ 
		SetCtrlVal (caratula, PANEL_Estado, 0);
		return 0;
	}

    /* Fecha Final */

	PromptPopup ("VALIDEZ", "Fecha Final (dd/mm/aa)", fecha, 8);
	
   	i = 10 * (fecha[0] - 48) +  fecha[1] - 48;
   	Int2Ascii (i,  letra);
   	Int2Ascii (35, dir);
	Fmt(comando, "%c%c%s%s%c",STX, WRMM, dir, letra, ETX);
	ComWrt (com, comando, 7);
	ans[0] = ComRdByte (com);
	if  (rs232err < 0){ 
		SetCtrlVal (caratula, PANEL_Estado, 0);
		return 0;
	}
	
   	i = 10 * (fecha[3] - 48) +  fecha[4] - 48;
   	Int2Ascii (i,  letra);
   	Int2Ascii (36, dir);
	Fmt(comando, "%c%c%s%s%c",STX, WRMM, dir, letra, ETX);
	ComWrt (com, comando, 7);
	ans[0] = ComRdByte (com);
	if  (rs232err < 0){ 
		SetCtrlVal (caratula, PANEL_Estado, 0);
		return 0;
	}

   	i = 10 * (fecha[6] - 48) +  fecha[7] - 48;
  	Int2Ascii (i,  letra);
   	Int2Ascii (37, dir);
	Fmt(comando, "%c%c%s%s%c",STX, WRMM, dir, letra, ETX);
	ComWrt (com, comando, 7);
	ans[0] = ComRdByte (com);
	if  (rs232err < 0){ 
		SetCtrlVal (caratula, PANEL_Estado, 0);
		return 0;
	}

	/* Lectura de 6 bytes a partir del 32 (período de validez) */

    Fmt(comando,"%s<%c%c%c%c%c%c%c",STX,RDMMN,'2','0','0','6',ETX);	
    ComWrt (com, comando, 7);				
	ComRd  (com, ans, 14);
	MostrarValidez();
	
	return 0;
} 

/************************************************************************************************************************/
/*																														*/
/*												Programa Principal														*/
/*																														*/
/************************************************************************************************************************/

int main(){

int	 	control;
char	comando[20];
char	puerto[5], texto[200];

/* Elección, apertura y configuración del puerto serie de comunicaciones */

com = GenericMessagePopup ("Control de Tarjetas", "¿PUERTO SERIE al que está conectada la placa?",
							"COM1", "COM2","", 0, 0, 0,
							VAL_GENERIC_POPUP_BTN1,
							VAL_GENERIC_POPUP_BTN1,
							VAL_GENERIC_POPUP_BTN3);

Fmt(puerto, "%s%i", "com",com);
puerto[4] = 0;
OpenComConfig (com, puerto, 9600, 2, 7, 1, 512, 512);

/* Enciende LED rojo */

Fmt (comando,"%s<%c%c%c",STX,BUSY,ETX);		
ComWrt (com, comando, 3);
ans[0] = ComRdByte (com);

/* Muestra cáratula principal */

caratula = LoadPanel (0, "expendedor.uir", PANEL);
DisplayPanel (caratula);

NuevaTarjeta();

while (1) {

	GetCtrlVal(caratula, PANEL_MasterPIN, &MasterPIN);
    GetUserEvent (1, &caratula, &control);
    switch (control) {

/***************************************** Salir ****************************************/

    	case PANEL_Salir:
			
			/*   Enciende LED rojo	*/    	
			
			Fmt(comando,"%s<%c%c%c",STX,BUSY,ETX);
 			ComWrt (com, comando, 3);
			ans[0] = ComRdByte (com);

			/*   Confirmación de salida	*/
			
			Fmt (texto, "%s", "¿Realmente está seguro de querer salir?");
			if ( ConfirmPopup ("Control de Tarjetas", texto) == 1 ) return 0;
    		break;

/**************************************** Acerca ****************************************/
    							
    	case PANEL_Acerca:
			
			Fmt (texto, "%s%s%s%s%s",	"Expendedor de Tarjetas + Recarga por PC\n\n",
										"Control de Acceso a un Parque: Uso de\n",
										"Tarjetas con Chip como Bonos de Prepago.\n\n",
										"Autor: Juan Manuel Gago Benítez\n",
										"PFC 2001/2002" );	
			MessagePopup ("Control de Tarjetas", texto);
    		break;

/************************************** NuevaTarjeta ************************************/

		case PANEL_NuevaTarjeta:
			
			NuevaTarjeta();
			break;

/************************************** AceptarPIN **************************************/

		case PANEL_AceptarPIN:
		
			AceptarPIN();
			break;
    							
/************************************* TarjetaVirgen ************************************/

		case PANEL_TarjetaVirgen:
			
			TarjetaVirgen();
			CambiarPIN();
			break;

/*************************************** Recargar ***************************************/

		case PANEL_Recargar:
		
			Recargar();
			break;
    							
/*************************************** CamiarDATOS ************************************/

		case PANEL_CambiarDATOS:
		
			CambiarDATOS();
			break;
		
/**************************************** CamiarPIN *************************************/

		case PANEL_CambiarPIN:
		
			CambiarPIN();
			break;

/************************************** CamiarVALIDEZ ***********************************/

		case PANEL_CambiarVALIDEZ:
		
			CambiarVALIDEZ();
			break;

    } 	/* fin del SWITCH */
    
}		/* fin del WHILE */

} 		/* fin del PROGRAMA PRINCIPAL */

