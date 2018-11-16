;***************************************************************************************;
;											;
;					CONFIG.inc					;
;											;
;			Subrutinas que actualizan la fecha y hora actual,		;
;		     aforo y número actual de vistantes y el PIN maestro.		;
;											;
;***************************************************************************************;

Config		MOVLW	0x01
		MOVWF	Linea		; Linea = 1

PintaConfig	MOVLW	CursOff		; Fin del parpadeo del cursor
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0
		MOVLW	LCDCLR		; Borra pantalla, cursor a casa
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0
		MOVLW	LCDLinea1	; Posiciona el cursor en la 1ª línea 
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0

		CLRF	Contador3
Coge_Letra10	MOVF	Contador3,0
		CALL	MenuConfig		; "* Menu Config. *"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje10
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra10

FinMensaje10	CALL	MostrarLinea

EscaneaConfig	CALL	KB_Port		; Escanea
		CALL	KB_Scan
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	EscaneaConfig
		CALL	LCD_Port_P0
		MOVF	Tecla,0

OK?		MOVLW	0x04
		SUBWF	Tecla,0		; ¿ Tecla = OK ?
		BTFSS	STATUS,Z
		GOTO	NO?
		CALL	EjecutarLinea
		GOTO	PintaConfig
		
NO?		MOVLW	0x03
		SUBWF	Tecla,0		; ¿ Tecla = NO ?
		BTFSS	STATUS,Z
		GOTO	ARRIBA?
		MOVLW	LCDCLR		; Borra pantalla, cursor a casa
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0
		RETURN

ARRIBA?		MOVLW	0x02
		SUBWF	Tecla,0		; ¿ Tecla = ARRIBA ?
		BTFSS	STATUS,Z
		GOTO	ABAJO?
		MOVLW	0x01
		SUBWF	Linea,0
		BTFSC	STATUS,Z
		GOTO	PonLinea_a_6
		DECF	Linea,1
		GOTO	PintaConfig
PonLinea_a_6	MOVLW	0x06
		MOVWF	Linea
		GOTO	PintaConfig

ABAJO?		MOVLW	0x01
		SUBWF	Tecla,0		; ¿ Tecla = ABAJO ?
		BTFSS	STATUS,Z
		GOTO	PintaConfig
		MOVLW	0x06
		SUBWF	Linea,0
		BTFSC	STATUS,Z
		GOTO	PonLinea_a_1
		INCF	Linea,1
		GOTO	PintaConfig
PonLinea_a_1	MOVLW	0x01
		MOVWF	Linea
		GOTO	PintaConfig

;******************************* Subrutina MostrarLinea *******************************
;
;		Muestra la línea actual determinada por el registro Linea
;
;**************************************************************************************

MostrarLinea	MOVLW	LCDLinea2	; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0

Mostrar1C?	MOVLW	0x01		; ¿ Linea = 1 ?
		SUBWF	Linea,0
		BTFSS	STATUS,Z
		GOTO	Mostrar2C?
		CLRF	Contador3
Coge_Letra11	MOVF	Contador3,0
		CALL	Linea1		; "1. Hora"
		IORLW	00h
		BTFSC	STATUS,Z
		RETURN
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra11

Mostrar2C?	MOVLW	0x02		; ¿ Linea = 2 ?
		SUBWF	Linea,0
		BTFSS	STATUS,Z
		GOTO	Mostrar3C?
		CLRF	Contador3
Coge_Letra12	MOVF	Contador3,0
		CALL	Linea2		; "2. Fecha"
		IORLW	00h
		BTFSC	STATUS,Z
		RETURN
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra12

Mostrar3C?	MOVLW	0x03		; ¿ Linea = 3 ?
		SUBWF	Linea,0
		BTFSS	STATUS,Z
		GOTO	Mostrar4C?
		CLRF	Contador3
Coge_Letra13	MOVF	Contador3,0
		CALL	Linea3		; "3. Aforo"
		IORLW	00h
		BTFSC	STATUS,Z
		RETURN
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra13

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Mostrar4C?	MOVLW	0x04		; ¿ Linea = 4 ?
		SUBWF	Linea,0
		BTFSS	STATUS,Z
		GOTO	Mostrar5C?
		MOVLW	"4"		; "4"
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVLW	"."		; "."
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVLW	" "		; " "
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		CLRF	Contador3
Coge_Letra14	MOVF	Contador3,0
		CALL	Visitante	; "Visitante: "
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje14
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra14
		
FinMensaje14	MOVLW	0xCC		; Posiciona el cursor en la 2ª fila 
		CALL	LCD_Comando_P0	; y en la 13ª columna del LCD
		CALL	Pausa_5ms_P0
		MOVLW	"s"		; "s"
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		RETURN

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Mostrar5C?	MOVLW	0x05		; ¿ Linea = 5 ?
		SUBWF	Linea,0
		BTFSS	STATUS,Z
		GOTO	Mostrar6C
		CLRF	Contador3
Coge_Letra15	MOVF	Contador3,0
		CALL	Linea5		; "5. PIN Maestro"
		IORLW	00h
		BTFSC	STATUS,Z
		RETURN
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra15

Mostrar6C	CLRF	Contador3	; Linea = 6
Coge_Letra16	MOVF	Contador3,0
		CALL	Linea6		; "6. Acerca de..."
		IORLW	00h
		BTFSC	STATUS,Z
		RETURN
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra16

;****************************** Subrutina EjecutarLinea *******************************
;
;		Ejecuta la línea actual determinada por el registro Linea
;
;**************************************************************************************

EjecutarLinea	MOVLW	LCDCLR		; Borra pantalla, cursor a casa
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0
		MOVLW	LCDLinea1	; Posiciona el cursor en la 1ª línea 
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0

Ejecutar1C?	MOVLW	0x01		; ¿ Linea = 1 ?
		SUBWF	Linea,0
		BTFSS	STATUS,Z
		GOTO	Ejecutar2C?
		CALL	Poner_Hora
		RETURN

Ejecutar2C?	MOVLW	0x02		; ¿ Linea = 2 ?
		SUBWF	Linea,0
		BTFSS	STATUS,Z
		GOTO	Ejecutar3C?
		CALL	Poner_Fecha
		RETURN

Ejecutar3C?	MOVLW	0x03		; ¿ Linea = 3 ?
		SUBWF	Linea,0
		BTFSS	STATUS,Z
		GOTO	Ejecutar4C?
		CALL	Poner_Aforo
		RETURN

Ejecutar4C?	MOVLW	0x04		; ¿ Linea = 4 ?
		SUBWF	Linea,0
		BTFSS	STATUS,Z
		GOTO	Ejecutar5C?
		CALL	Poner_Visitas
		RETURN

Ejecutar5C?	MOVLW	0x05		; ¿ Linea = 5 ?
		SUBWF	Linea,0
		BTFSS	STATUS,Z
		GOTO	Ejecutar6C
		CALL	PIN_Maestro
		RETURN

Ejecutar6C	CALL 	Informar	;   Linea = 6
		RETURN


