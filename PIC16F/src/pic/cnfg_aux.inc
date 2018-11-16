;***************************************************************************************;
;											;
;					CNFG_AUX.inc					;
;											;
;			      Subrutinas auxiliares de CONFIG.inc			;
;											;
;	Modifica las variables globales M1, D1 y temp					;
;											;
;***************************************************************************************;

;******************************** Subrutina Poner_Hora *********************************
;
;			    Actualiza los registros hh y mm
;
;***************************************************************************************

Poner_Hora	CLRF	Contador3

Coge_Letra20	MOVF	Contador3,0
		CALL	FormatoHora	; "      hh:mm"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje20
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra20

FinMensaje20	MOVLW	0xC5		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 6ª columna del LCD
		CALL	Pausa_5ms_P0

		MOVF	hh,0		; Muestra hh:
		CALL	Bin8BCD
		MOVLW	30h
		ADDWF	Decenas,1
		ADDWF	Unidades,1
		MOVF	Decenas,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVLW	':'		
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		MOVF	mm,0		; Muestra mm
		CALL	Bin8BCD
		MOVLW	30h
		ADDWF	Decenas,1
		ADDWF	Unidades,1
		MOVF	Decenas,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		MOVLW	CursBlink	; Parpadeo del cursor
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0

Analiza_h1	MOVLW	0xC5		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 6ª columna del LCD
		CALL	Pausa_5ms_P0
		CALL	KB_Port	
Escanea_h1	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_h1
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; ¿ Tecla < 5 ?
		BTFSS	STATUS,C
		GOTO	h1_Dcha?
		GOTO	h1_Numero

h1_Dcha?	MOVLW	0x01
		SUBWF	Tecla,0		; ¿ Tecla = Izda ?
		BTFSS	STATUS,Z
		GOTO	h1_Izda?
		GOTO	Analiza_m1

h1_Izda?	MOVLW	0x02
		SUBWF	Tecla,0		; ¿  Tecla = Dcha ?
		BTFSS	STATUS,Z
		RETURN			; Si Tecla = OK ó NO, RETURN
		GOTO	Analiza_m1

h1_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	Decenas

		MOVLW	0x03
		SUBWF	Decenas,0	; ¿ Decenas < 3 ?
		BTFSC	STATUS,C
		GOTO	Analiza_h1

		MOVF	Decenas,0
		MOVWF	D1
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		CALL	Decenasx10
		MOVWF	hh

Analiza_h0	CALL	KB_Port	
Escanea_h0	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_h0
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; Si Tecla < 5 no se ha podido actualizar
		BTFSS	STATUS,C	; hh luego no estará permitido
		GOTO	Analiza_h0

h0_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	Unidades

		MOVLW	0x02
		SUBWF	D1,0		; ¿ h1 = 2 ?
		BTFSS	STATUS,Z
		GOTO	h0_bueno
		MOVLW	0x04
		SUBWF	Unidades,0	; ¿ Unidades < 4 ?
		BTFSC	STATUS,C
		GOTO	Analiza_h0

h0_bueno	MOVF	Unidades,0
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		ADDWF	hh,1

Analiza_m1	MOVLW	0xC8		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 9ª columna del LCD
		CALL	Pausa_5ms_P0
		CALL	KB_Port	
Escanea_m1	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_m1
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; ¿ Tecla < 5 ?
		BTFSS	STATUS,C
		GOTO	m1_Dcha?
		GOTO	m1_Numero

m1_Dcha?	MOVLW	0x01
		SUBWF	Tecla,0		; ¿ Tecla = Izda ?
		BTFSS	STATUS,Z
		GOTO	m1_Izda?
		GOTO	Analiza_h1

m1_Izda?	MOVLW	0x02
		SUBWF	Tecla,0		; ¿  Tecla = Dcha ?
		BTFSS	STATUS,Z
		RETURN			; Si Tecla = OK ó NO, RETURN
		GOTO	Analiza_h1

m1_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	Decenas

		MOVLW	0x06
		SUBWF	Decenas,0	; ¿ Decenas < 6 ?
		BTFSC	STATUS,C
		GOTO	Analiza_m1

		MOVF	Decenas,0
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		CALL	Decenasx10
		MOVWF	mm

Analiza_m0	CALL	KB_Port	
Escanea_m0	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_m0
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; Si Tecla < 5 no se ha podido actualizar
		BTFSS	STATUS,C	; mm luego no estará permitido
		GOTO	Analiza_m0

m0_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	Unidades
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		ADDWF	mm,1
		GOTO	Analiza_h1

;******************************** Subrutina Poner_Fecha ********************************
;
;    Actualiza los registros AA, MM y DD y las posiciones 4, 5 y 6 de la EEPROM
;
;***************************************************************************************

Poner_Fecha	CLRF	Contador3

Coge_Letra21	MOVF	Contador3,0
		CALL	FormatoFecha	; "    aa/mm/dd"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje21
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra21

FinMensaje21	MOVLW	0xC4		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 5ª columna del LCD
		CALL	Pausa_5ms_P0

		movlw	0x04
		call	Leer_EE		; Muestra AA/
		movwf	AA
		call	Bin8BCD
		MOVLW	30h
		ADDWF	Decenas,1
		ADDWF	Unidades,1
		MOVF	Decenas,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVLW	'/'
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		movlw	0x05
		call	Leer_EE		; Muestra MM/
		movwf	MM
		call	Bin8BCD
		MOVLW	30h
		ADDWF	Decenas,1
		ADDWF	Unidades,1
		MOVF	Decenas,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVLW	'/'
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		movlw	0x06
		call	Leer_EE		; Muestra DD/
		movwf	DD
		call	Bin8BCD
		MOVLW	30h
		ADDWF	Decenas,1
		ADDWF	Unidades,1
		MOVF	Decenas,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		MOVLW	CursBlink	; Parpadeo del cursor
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0

Analiza_A1	MOVLW	0xC4		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 5ª columna del LCD
		CALL	Pausa_5ms_P0
		CALL	KB_Port	
Escanea_A1	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_A1
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; ¿ Tecla < 5 ?
		BTFSS	STATUS,C
		GOTO	A1_OK?
		GOTO	A1_Numero

A1_OK?		MOVLW	0x04
		SUBWF	Tecla,0		; ¿ Tecla = OK ?
		BTFSS	STATUS,Z
		GOTO	A1_NO?
		MOVF	AA,0
		MOVWF	VALUE
		MOVLW	0x04
		MOVWF	ADDR
		CALL	Escribir_EE
		MOVF	MM,0
		MOVWF	VALUE
		MOVLW	0x05
		MOVWF	ADDR
		CALL	Escribir_EE
		MOVF	DD,0
		MOVWF	VALUE
		MOVLW	0x06
		MOVWF	ADDR
		CALL	Escribir_EE
		RETURN

A1_NO?		MOVLW	0x03
		SUBWF	Tecla,0		; ¿ Tecla = NO ?
		BTFSC	STATUS,Z
		RETURN

A1_Izda?	MOVLW	0x02
		SUBWF	Tecla,0		; ¿ Tecla = Izda ?
		BTFSS	STATUS,Z
		GOTO	A1_Dcha
		GOTO	Analiza_D1

A1_Dcha		GOTO	Analiza_M1	;   Tecla = Dcha

A1_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	Decenas
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		CALL	Decenasx10
		MOVWF	AA

Analiza_A0	CALL	KB_Port	
Escanea_A0	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_A0
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; Si Tecla < 5 no se ha podido actualizar
		BTFSS	STATUS,C	; AA luego no estará permitido
		GOTO	Analiza_A0

A0_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	Unidades
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		ADDWF	AA,1

Analiza_M1	MOVLW	0xC7		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 8ª columna del LCD
		CALL	Pausa_5ms_P0
		CALL	KB_Port	
Escanea_M1	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_M1
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; ¿ Tecla < 5 ?
		BTFSS	STATUS,C
		GOTO	M1_OK?
		GOTO	M1_Numero

M1_OK?		MOVLW	0x04
		SUBWF	Tecla,0		; ¿ Tecla = OK ?
		BTFSS	STATUS,Z
		GOTO	M1_NO?
		MOVF	AA,0
		MOVWF	VALUE
		MOVLW	0x04
		MOVWF	ADDR
		CALL	Escribir_EE
		MOVF	MM,0
		MOVWF	VALUE
		MOVLW	0x05
		MOVWF	ADDR
		CALL	Escribir_EE
		MOVF	DD,0
		MOVWF	VALUE
		MOVLW	0x06
		MOVWF	ADDR
		CALL	Escribir_EE
		RETURN

M1_NO?		MOVLW	0x03
		SUBWF	Tecla,0		; ¿ Tecla = NO ?
		BTFSC	STATUS,Z
		RETURN

M1_Izda?	MOVLW	0x02
		SUBWF	Tecla,0		; ¿ Tecla = Izda ?
		BTFSS	STATUS,Z
		GOTO	M1_Dcha
		GOTO	Analiza_A1

M1_Dcha		GOTO	Analiza_D1	;   Tecla = Dcha

M1_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	Decenas

		MOVLW	0x02
		SUBWF	Decenas,0	; ¿ Decenas < 2 ?
		BTFSC	STATUS,C
		GOTO	Analiza_M1

		MOVF	Decenas,0
		MOVWF	M1
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		CALL	Decenasx10
		MOVWF	MM

Analiza_M0	CALL	KB_Port	
Escanea_M0	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_M0
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; Si Tecla < 5 no se ha podido actualizar
		BTFSS	STATUS,C	; MM luego no estará permitido
		GOTO	Analiza_M0

M0_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	Unidades

		DECF	M1,0		; ¿  M1 = 1 ?
		BTFSS	STATUS,Z
		GOTO	M0_POS?		; Si M1 = 0, ¿ M0 = 0 ?
		MOVLW	0x03
		SUBWF	Unidades,0	; ¿ Unidades < 3 ?
		BTFSC	STATUS,C
		GOTO	Analiza_M0
		GOTO	M0_bueno

M0_POS?		MOVF	Unidades,1	; ¿ Unidades = 0 ? 
		BTFSC	STATUS,Z
		GOTO	Analiza_M0

M0_bueno	MOVF	Unidades,0
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		ADDWF	MM,1

Analiza_D1	MOVLW	0xCA		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 11ª columna del LCD
		CALL	Pausa_5ms_P0
		CALL	KB_Port	
Escanea_D1	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_D1
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; ¿ Tecla < 5 ?
		BTFSS	STATUS,C
		GOTO	D1_OK?
		GOTO	D1_Numero

D1_OK?		MOVLW	0x04
		SUBWF	Tecla,0		; ¿ Tecla = OK ?
		BTFSS	STATUS,Z
		GOTO	D1_NO?
		MOVF	AA,0
		MOVWF	VALUE
		MOVLW	0x04
		MOVWF	ADDR
		CALL	Escribir_EE
		MOVF	MM,0
		MOVWF	VALUE
		MOVLW	0x05
		MOVWF	ADDR
		CALL	Escribir_EE
		MOVF	DD,0
		MOVWF	VALUE
		MOVLW	0x06
		MOVWF	ADDR
		CALL	Escribir_EE
		RETURN

D1_NO?		MOVLW	0x03
		SUBWF	Tecla,0		; ¿ Tecla = NO ?
		BTFSC	STATUS,Z
		RETURN

D1_Izda?	MOVLW	0x02
		SUBWF	Tecla,0		; ¿ Tecla = Izda ?
		BTFSS	STATUS,Z
		GOTO	D1_Dcha
		GOTO	Analiza_M1

D1_Dcha		GOTO	Analiza_A1	;   Tecla = Dcha

D1_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	Decenas

		MOVLW	0x02
		SUBWF	MM,0		; ¿ MM = 2 ?
		BTFSS	STATUS,Z
		GOTO	No_Febrero
Febrero		MOVLW	0x03
		SUBWF	Decenas,0	; ¿ Decenas < 3 ?
		BTFSC	STATUS,C
		GOTO	Analiza_D1
		GOTO	D1_bueno

No_Febrero	MOVLW	0x04
		SUBWF	Decenas,0	; ¿ Decenas < 4 ?
		BTFSC	STATUS,C
		GOTO	Analiza_D1

D1_bueno	MOVF	Decenas,0
		MOVWF	D1
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		CALL	Decenasx10
		MOVWF	DD

Analiza_D0	CALL	KB_Port	
Escanea_D0	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_D0
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; Si Tecla < 5 no se ha podido actualizar
		BTFSS	STATUS,C	; DD luego no estará permitido
		GOTO	Analiza_D0

D0_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	Unidades

		MOVF	D1,1		; ¿ D1 = 0 ?
		BTFSC	STATUS,Z
		GOTO	D0_positivo?	; Si D1 = 0, ¿ D0 = 0 ? 
		
		MOVLW	0x02
		SUBWF	D1,0		; ¿ D1 = 2 ?
		BTFSS	STATUS,Z
		GOTO	D1_es_3?
		MOVLW	0x02
		SUBWF	MM,0		; ¿ MM = 2 ?
		BTFSS	STATUS,Z
		GOTO	D1_es_3?
		BTFSC	AA,0		; ¿ AA % 4 = 0 ?
		GOTO	No_Bisiesto
		BTFSC	AA,1
		GOTO	No_Bisiesto
		MOVLW	0x0A
		SUBWF	Unidades,0	; ¿ Unidades < 10 ?
		BTFSC	STATUS,C
		GOTO	Analiza_D0
		GOTO	D0_bueno

No_Bisiesto	MOVLW	0x09
		SUBWF	Unidades,0	; ¿ Unidades <  9 ?
		BTFSC	STATUS,C
		GOTO	Analiza_D0
		GOTO	D0_bueno
		
D1_es_3?	MOVLW	0x03
		SUBWF	D1,0		; ¿ D1 = 3 ?
		BTFSS	STATUS,Z
		GOTO	D0_bueno
		MOVLW	0x04
		SUBWF	MM,0		; ¿ MM = 4 ?
		BTFSC	STATUS,Z
		GOTO	D0_es_0?
		MOVLW	0x06
		SUBWF	MM,0		; ¿ MM = 6 ?
		BTFSC	STATUS,Z
		GOTO	D0_es_0?
		MOVLW	0x09
		SUBWF	MM,0		; ¿ MM = 9 ?
		BTFSC	STATUS,Z
		GOTO	D0_es_0?
		MOVLW	0x0B
		SUBWF	MM,0		; ¿ MM = 11 ?
		BTFSC	STATUS,Z
		GOTO	D0_es_0?
Resto_Meses	MOVLW	0x02
		SUBWF	Unidades,0	; ¿ Unidades < 2 ?
		BTFSC	STATUS,C
		GOTO	Analiza_D0
		GOTO	D0_bueno

D0_es_0?	MOVF	Unidades,1	; ¿ Unidades = 0 ? 
		BTFSS	STATUS,Z
		GOTO	Analiza_D0
		GOTO	D0_bueno

D0_positivo?	MOVF	Unidades,1	; ¿ Unidades = 0 ? 
		BTFSC	STATUS,Z
		GOTO	Analiza_D0

D0_bueno	MOVF	Unidades,0
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		ADDWF	DD,1

		GOTO	Analiza_A1

;******************************** Subrutina Poner_Aforo *********************************
;
; 		      Actualiza las posiciones 2 y 3 de la EEPROM
;
;***************************************************************************************

Poner_Aforo	MOVLW	0x85		; Posiciona el cursor en la 1ª fila
		CALL	LCD_Comando_P0	; y en la 6ª columna del LCD
		CALL	Pausa_5ms_P0

		MOVLW	0x03
		MOVWF	Contador3
Coge_Letra22	MOVF	Contador3,0
		CALL	Linea3		; "Aforo"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje22
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra22
FinMensaje22	MOVLW	":"
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		MOVLW	0x02
		CALL	Leer_EE
		MOVWF	L_byte		; L_byte = EE(2)
		MOVLW	0x03
		CALL	Leer_EE
		MOVWF	H_byte		; H_byte = EE(3)

Pinta_Aforo	MOVLW	0xC1		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 2ª columna del LCD
		CALL	Pausa_5ms_P0

		CALL	Bin16BCD	; R0, R1, R2 = Dígitos BCD
					; H_byte = L_byte = 0
		MOVLW	0x30
		ADDWF	R0,0		; Muestra Decenas de Millar
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		SWAPF	R1,0
		ANDLW	0x0F
		ADDLW	0x30		; Muestra Unidades de Millar
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		
		MOVF	R1,0
		ANDLW	0x0F
		ADDLW	0x30		; Muestra Centenas
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		SWAPF	R2,0
		ANDLW	0x0F
		ADDLW	0x30		; Muestra Decenas
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		MOVF	R2,0
		ANDLW	0x0F
		ADDLW	0x30		; Muestra Unidades
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		CLRF	Contador3
Coge_Letra23	MOVF	Contador3,0
		CALL	Personas	; "  personas"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje23
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra23

FinMensaje23	MOVLW	CursBlink	; Parpadeo del cursor
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0

		MOVLW	0x02
		MOVWF	Dir1
		MOVLW	0x03
		MOVWF	Dir2
		GOTO	Analiza_DM

;******************************* Subrutina Poner_Visitas ********************************
;
; 		      Actualiza las posiciones 0 y 1 de la EEPROM
;
;***************************************************************************************

Poner_Visitas	MOVLW	0x83		; Posiciona el cursor en la 1ª fila
		CALL	LCD_Comando_P0	; y en la 4ª columna del LCD
		CALL	Pausa_5ms_P0

		CLRF	Contador3
Coge_Letra24	MOVF	Contador3,0
		CALL	Visitante		; "Visitante: "
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje24
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra24
FinMensaje24	MOVLW	0x8C		; Posiciona el cursor en la 1ª fila 
		CALL	LCD_Comando_P0	; y en la 13ª columna del LCD
		CALL	Pausa_5ms_P0
		MOVLW	"s"		; "s"
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVLW	":"		; ":"
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		MOVLW	0x00
		CALL	Leer_EE	
		MOVWF	L_byte		; L_byte = EE(0)
		MOVLW	0x01
		CALL	Leer_EE	
		MOVWF	H_byte		; H_byte = EE(1)

Pinta_Visita	MOVLW	0xC1		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 2ª columna del LCD
		CALL	Pausa_5ms_P0

		CALL	Bin16BCD	; R0, R1, R2 = Dígitos BCD
					; H_byte = L_byte = 0
		MOVLW	0x30
		ADDWF	R0,0		; Muestra Decenas de Millar
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		SWAPF	R1,0
		ANDLW	0x0F
		ADDLW	0x30		; Muestra Unidades de Millar
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		
		MOVF	R1,0
		ANDLW	0x0F
		ADDLW	0x30		; Muestra Centenas
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		SWAPF	R2,0
		ANDLW	0x0F
		ADDLW	0x30		; Muestra Decenas
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		MOVF	R2,0
		ANDLW	0x0F
		ADDLW	0x30		; Muestra Unidades
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		CLRF	Contador3
Coge_Letra25	MOVF	Contador3,0
		CALL	Personas	; "  personas"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje25
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra25

FinMensaje25	MOVLW	CursBlink	; Parpadeo del cursor
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0

		CLRF	Dir1
		MOVLW	0x01
		MOVWF	Dir2
		GOTO	Analiza_DM

;********************************* Sección Analiza_DM ***********************************
;
;      Escanea 5 dígitos, los convierte en un número de 16 bits y lo guarda en las 
; direcciones Dir1 y Dir2 de la EEPROM. Forma parte de Poner_Aforo y Poner_Visitas.
;
;****************************************************************************************

Analiza_DM	MOVLW	0xC1		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 2ª columna del LCD
		CALL	Pausa_5ms_P0
		CALL	KB_Port	
Escanea_DM	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_DM
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; ¿ Tecla < 5 ?
		BTFSS	STATUS,C
		GOTO	DM_OK?
		GOTO	DM_Numero

DM_OK?		MOVLW	0x04
		SUBWF	Tecla,0		; ¿ Tecla = OK ?
		BTFSS	STATUS,Z
		GOTO	DM_NO?
		call	BCDBin16
		movf	L_byte,0
		movwf	VALUE	
		movf	Dir1,0
		movwf	ADDR
		call	Escribir_EE
		movf	H_byte,0
		movwf	VALUE
		movf	Dir2,0
		movwf	ADDR
		call	Escribir_EE
		RETURN

DM_NO?		MOVLW	0x03
		SUBWF	Tecla,0		; ¿ Tecla = NO ?
		BTFSC	STATUS,Z
		RETURN

DM_Izda?	MOVLW	0x02
		SUBWF	Tecla,0		; ¿ Tecla = Izda ?
		BTFSS	STATUS,Z
		GOTO	DM_Dcha
		GOTO	Analiza_U

DM_Dcha		GOTO	Analiza_UM	;   Tecla = Dcha

DM_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	R0
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

Analiza_UM	MOVLW	0xC2		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 3ª columna del LCD
		CALL	Pausa_5ms_P0
		CALL	KB_Port	
Escanea_UM	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_UM
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; ¿ Tecla < 5 ?
		BTFSS	STATUS,C
		GOTO	UM_OK?
		GOTO	UM_Numero

UM_OK?		MOVLW	0x04
		SUBWF	Tecla,0		; ¿ Tecla = OK ?
		BTFSS	STATUS,Z
		GOTO	UM_NO?
		call	BCDBin16
		movf	L_byte,0
		movwf	VALUE
		movf	Dir1,0
		movwf	ADDR
		call	Escribir_EE
		movf	H_byte,0
		movwf	VALUE
		movf	Dir2,0
		movwf	ADDR
		call	Escribir_EE
		RETURN

UM_NO?		MOVLW	0x03
		SUBWF	Tecla,0		; ¿ Tecla = NO ?
		BTFSC	STATUS,Z
		RETURN

UM_Izda?	MOVLW	0x02
		SUBWF	Tecla,0		; ¿ Tecla = Izda ?
		BTFSS	STATUS,Z
		GOTO	UM_Dcha
		GOTO	Analiza_DM

UM_Dcha		GOTO	Analiza_C	;   Tecla = Dcha

UM_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	temp
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVLW	0x0F
		ANDWF	R1,1		
		SWAPF	temp,0
		IORWF	R1,1

Analiza_C	MOVLW	0xC3		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 4ª columna del LCD
		CALL	Pausa_5ms_P0
		CALL	KB_Port	
Escanea_C	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_C
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; ¿ Tecla < 5 ?
		BTFSS	STATUS,C
		GOTO	C_OK?
		GOTO	C_Numero

C_OK?		MOVLW	0x04
		SUBWF	Tecla,0		; ¿ Tecla = OK ?
		BTFSS	STATUS,Z
		GOTO	C_NO?
		call	BCDBin16
		movf	L_byte,0
		movwf	VALUE
		movf	Dir1,0
		movwf	ADDR
		call	Escribir_EE
		movf	H_byte,0
		movwf	VALUE
		movf	Dir2,0
		movwf	ADDR
		call	Escribir_EE
		RETURN

C_NO?		MOVLW	0x03
		SUBWF	Tecla,0		; ¿ Tecla = NO ?
		BTFSC	STATUS,Z
		RETURN

C_Izda?		MOVLW	0x02
		SUBWF	Tecla,0		; ¿ Tecla = Izda ?
		BTFSS	STATUS,Z
		GOTO	C_Dcha
		GOTO	Analiza_UM

C_Dcha		GOTO	Analiza_D	;   Tecla = Dcha

C_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	temp
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVLW	0xF0
		ANDWF	R1,1
		MOVF	temp,0
		IORWF	R1,1

Analiza_D	MOVLW	0xC4		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 5ª columna del LCD
		CALL	Pausa_5ms_P0
		CALL	KB_Port	
Escanea_D	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_D
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; ¿ Tecla < 5 ?
		BTFSS	STATUS,C
		GOTO	D_OK?
		GOTO	D_Numero

D_OK?		MOVLW	0x04
		SUBWF	Tecla,0		; ¿ Tecla = OK ?
		BTFSS	STATUS,Z
		GOTO	D_NO?
		call	BCDBin16
		movf	L_byte,0
		movwf	VALUE
		movf	Dir1,0
		movwf	ADDR
		call	Escribir_EE
		movf	H_byte,0
		movwf	VALUE
		movf	Dir2,0
		movwf	ADDR
		call	Escribir_EE
		RETURN

D_NO?		MOVLW	0x03
		SUBWF	Tecla,0		; ¿ Tecla = NO ?
		BTFSC	STATUS,Z
		RETURN

D_Izda?		MOVLW	0x02
		SUBWF	Tecla,0		; ¿ Tecla = Izda ?
		BTFSS	STATUS,Z
		GOTO	D_Dcha
		GOTO	Analiza_C

D_Dcha		GOTO	Analiza_U	;   Tecla = Dcha

D_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	temp
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVLW	0x0F
		ANDWF	R2,1		
		SWAPF	temp,0
		IORWF	R2,1

Analiza_U	MOVLW	0xC5		; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0	; y en la 6ª columna del LCD
		CALL	Pausa_5ms_P0
		CALL	KB_Port	
Escanea_U	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	Escanea_U
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; ¿ Tecla < 5 ?
		BTFSS	STATUS,C
		GOTO	U_OK?
		GOTO	U_Numero

U_OK?		MOVLW	0x04
		SUBWF	Tecla,0		; ¿ Tecla = OK ?
		BTFSS	STATUS,Z
		GOTO	U_NO?
		call	BCDBin16
		movf	L_byte,0
		movwf	VALUE
		movf	Dir1,0
		movwf	ADDR
		call	Escribir_EE
		movf	H_byte,0
		movwf	VALUE
		movf	Dir2,0
		movwf	ADDR
		call	Escribir_EE
		RETURN

U_NO?		MOVLW	0x03
		SUBWF	Tecla,0		; ¿ Tecla = NO ?
		BTFSC	STATUS,Z
		RETURN

U_Izda?		MOVLW	0x02
		SUBWF	Tecla,0		; ¿ Tecla = Izda ?
		BTFSS	STATUS,Z
		GOTO	U_Dcha
		GOTO	Analiza_D

U_Dcha		GOTO	Analiza_DM	;   Tecla = Dcha

U_Numero	MOVF	Tecla,0 
		CALL	Conversion
		MOVWF	temp
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVLW	0xF0
		ANDWF	R2,1
		MOVF	temp,0
		IORWF	R2,1
		GOTO	Analiza_DM

;******************************** Subrutina PIN_Maestro ********************************
;
; 		    Actualiza las posiciones 7, 8 y 9 de la EEPROM
;
;***************************************************************************************

PIN_Maestro	CLRF	Contador3

Coge_Letra26	MOVLW	0x08		; Necesario para poder acceder
		MOVWF	PCLATH		;  correctamente a una tabla
		MOVF	Contador3,0
		CALL	Nuevo_PIN	; " Nuevo PIN: "
		CLRF	PCLATH		;  Selecciona página 0
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje26
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra26

FinMensaje26	MOVF	MASTER_PIN1,0
		CALL	Conversion
		ADDLW	0x30
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		MOVF	MASTER_PIN2,0
		CALL	Conversion
		ADDLW	0x30
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		MOVF	MASTER_PIN3,0
		CALL	Conversion
		ADDLW	0x30
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		MOVLW	0x8C		; Posiciona el cursor en la 1ª fila
		CALL	LCD_Comando_P0	; y en la 13ª columna del LCD
		CALL	Pausa_5ms_P0
		MOVLW	CursBlink	; Parpadeo del cursor
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0

ScanMASTER1	CALL	KB_Port
		CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	ScanMASTER1
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; Tecla < 5  no
		BTFSS	STATUS,C	; estará permitido
		GOTO	ScanMASTER1

		MOVF	Tecla,0		; EE(7) = MASTER_PIN1
		MOVWF	MASTER_PIN1
		MOVWF	VALUE
		MOVLW 	0x07
		MOVWF	ADDR
		call	Escribir_EE

		MOVF	VALUE,0		; y lo muestra en el LCD
		CALL	Conversion
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

ScanMASTER2	CALL	KB_Port
		CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	ScanMASTER2
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; Tecla < 5  no
		BTFSS	STATUS,C	; estará permitido
		GOTO	ScanMASTER2

		MOVF	Tecla,0		; EE(8) = MASTER_PIN2
		MOVWF	MASTER_PIN2
		MOVWF	VALUE
		MOVLW 	0x08
		MOVWF	ADDR
		call	Escribir_EE

		MOVF	VALUE,0		; y lo muestra en el LCD
		CALL	Conversion
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

ScanMASTER3	CALL	KB_Port
		CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	ScanMASTER3
		CALL	LCD_Port_P0
		MOVLW	0x05
		SUBWF	Tecla,0		; Tecla < 5  no
		BTFSS	STATUS,C	; estará permitido
		GOTO	ScanMASTER3

		MOVF	Tecla,0		; EE(9) = MASTER_PIN3
		MOVWF	MASTER_PIN3
		MOVWF	VALUE
		MOVLW 	0x09
		MOVWF	ADDR
		call	Escribir_EE

		MOVF	VALUE,0		; y lo muestra en el LCD
		CALL	Conversion
		ADDLW	30h
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		RETURN

;********************************* Subrutina Informar *********************************
;
;			    Muestra información del proyecto
;
;***************************************************************************************

Informar	MOVLW	LCDLinea1
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0

		CLRF	Contador3
Coge_Letra27	MOVLW	0x0F		; Necesario para poder acceder
		MOVWF	PCLATH		;  correctamente a una tabla
		MOVF	Contador3,0
		CALL	Informacion1	; "Control de Acceso. Juan ..."
		CLRF	PCLATH		;  Selecciona página 0
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje27
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra27

FinMensaje27	MOVLW	LCDLinea2
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0

		CLRF	Contador3
Coge_Letra28	MOVLW	0x0F		; Necesario para poder acceder
		MOVWF	PCLATH		;  correctamente a una tabla
		MOVF	Contador3,0
		CALL	Informacion2	; " PFC 2001/2002. Pulse ..."
		CLRF	PCLATH		;  Selecciona página 0
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	Desplazar
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra28

Desplazar	CALL	LCD_Port_P0
		MOVLW	LCDIzda
		CALL	LCD_Comando_P0
		CALL	Pausa_500ms
		CALL	Pausa_500ms	; Retardo necesario
		CALL	Pausa_500ms	;  para poder leer

		CALL	KB_Port		
		MOVLW	b'00001110'
		MOVWF	PORTB
		BTFSC	PORTB,OK	; Si no se ha pulsado OK, Desplazar
		GOTO	Desplazar	

		CALL	LCD_Port_P0
		RETURN
	
