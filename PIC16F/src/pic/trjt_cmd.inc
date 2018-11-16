;***************************************************************************************;
;											;
;					TRJT_CMD.inc					;
;											;
;	Librería de subrutinas de comandos que leen o escriben en la tarjeta. 		;
;		  Por tanto están muy ligadas a la librería SLE4442.inc			;
;											;
;	RecargaSaldo modifica las variables globales A_fin, M_fin, Dir1, Dir2		;
;											;
;***************************************************************************************;

;********************************** Subrutina LeerEC **********************************
;
;    Actualiza el registro EC con el estado del contador de errores de la tarjeta
;
;**************************************************************************************

LeerEC		call	BSTART
		movlw	0x31		; Mando el comando para 
		movwf	txbuf		; leer la Security Memory
		call	TX
		movlw	0x00
		movwf	txbuf
		call	TX
		movlw	0x00
		movwf	txbuf
		call	TX
		call	BSTOP
		
		call	RX		; Guardo sólo el EC
		movf	datai,0		; ya que el PSC puede ser engañoso
		movwf	EC		
		call	RX
		call	RX
		call	RX
		call	RX
		return

;********************************** Subrutina CompPSC *********************************
;
;      Realiza el comando 'Verificación del PSC' como se indica en el datasheet
;
;**************************************************************************************

CompPSC		call	Quita1intento
		call	BSTART
		movlw	0x39		; Mando el comando para escribir
		movwf	txbuf		; en la Security Memory
		call	TX
		movlw	0x00
		movwf	txbuf
		call	TX
		movf	EC,0		; EC = EC - 1
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing

		call	BSTART
		movlw	0x33		; Mando el comando para comparar
		movwf	txbuf		; el 1º byte del PSC
		call	TX
		movlw	0x01
		movwf	txbuf
		call	TX
		movf	PIN1,0
		movwf	txbuf
		call	TX
		call	BSTOP
		call	RX		; 8 pulsos de CLK para procesar

		call	BSTART
		movlw	0x33		; Mando el comando para comparar
		movwf	txbuf		; el 2º byte del PSC
		call	TX
		movlw	0x02
		movwf	txbuf
		call	TX
		movf	PIN2,0
		movwf	txbuf
		call	TX
		call	BSTOP
		call	RX		; 8 pulsos de CLK para procesar

		call	BSTART
		movlw	0x33		; Mando el comando para comparar
		movwf	txbuf		; el 3º byte del PSC
		call	TX
		movlw	0x03
		movwf	txbuf
		call	TX
		movf	PIN3,0
		movwf	txbuf
		call	TX
		call	BSTOP
		call	RX		; 8 pulsos de CLK para procesar

		call	BSTART
		movlw	0x39		; Mando el comando para escribir
		movwf	txbuf		; en la Security Memory
		call	TX
		movlw	0x00
		movwf	txbuf
		call	TX
		movlw	0xFF		; EC = 7
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing

		return

;****************************** Subrutina Escribe_Hoy *********************************
;
;	   Tras pulsar la opción "1. Entrar" del Menú de Usuario, escribe en la  
;         tarjeta la fecha de hoy en el caso de que haya que cobrar (Cobrar = 1)
;
;**************************************************************************************

Escribe_Hoy	call	BSTART
		movlw	0x38		; Mando el comando para escribir
		movwf	txbuf		; en la Main Memory
		call	TX
		movf	FSR,0		; FSR = dir. de la tarjeta donde registrar DD 
		movwf	txbuf
		call	TX
		movf	DD,0		; Escribo DD
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing
		incf	FSR,1		

		call	BSTART
		movlw	0x38
		movwf	txbuf
		call	TX
		movf	FSR,0		; FSR = dir. de la tarjeta donde registrar MM
		movwf	txbuf
		call	TX
		movf	MM,0		; Escribo MM
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing
		incf	FSR,1		

		call	BSTART
		movlw	0x38
		movwf	txbuf
		call	TX
		movf	FSR,0		; FSR = dir. de la tarjeta donde registrar AA 
		movwf	txbuf
		call	TX
		movf	AA,0		; Escribo AA
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing

		return

;***************************** Subrutina RecargarSaldo ********************************
;			    Opción 1 del Menú de Usuario
;**************************************************************************************

RecargarSaldo	MOVLW	LCDCLR		; Borra pantalla, cursor a casa
		CALL	LCD_Comando_P1
		CALL	Pausa_5ms_P1

		CLRF	Contador3
Coge_Letra50	MOVF	Contador3,0
		CALL	Recargando	; "   Recargando"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje50
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra50

FinMensaje50	MOVLW	0xC3		; Posiciona el cursor en la 2ª línea
		CALL	LCD_Comando_P1	; y en la 4ª columna
		CALL	Pausa_5ms_P1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

		call	BSTART
		movlw	0x38		; Mando el comando para escribir
		movwf	txbuf		; en la Main Memory
		call	TX
		movlw   0x20		; dir. de la tarjeta donde registrar D_ini 
		movwf	txbuf
		call	TX
		movf	DD,0		; Escribo DD
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing

		call	BSTART
		movlw	0x38
		movwf	txbuf
		call	TX
		movlw	0x21		; dir. de la tarjeta donde registrar M_ini
		movwf	txbuf
		call	TX
		movf	MM,0		; Escribo MM
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing

		call	BSTART
		movlw	0x38
		movwf	txbuf
		call	TX
		movlw	0x22		; dir. de la tarjeta donde registrar A_ini
		movwf	txbuf
		call	TX
		movf	AA,0		; Escribo AA
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

		movf	AA,0
		movwf	A_fin		; A_fin = AA
		movf	MM,0		; M_fin = MM
		movwf	M_fin
		sublw	.12	
		btfss	STATUS,Z
		goto 	Inc_M_fin	
		movlw	.1
		movwf	M_fin		; Si M_fin = 12, M_fin = 1
		incf	A_fin,1		; y  A_fin = A_fin + 1
		goto	$+2
Inc_M_fin	incf	M_fin,1

		call	BSTART
		movlw	0x38		; Mando el comando para escribir
		movwf	txbuf		; en la Main Memory
		call	TX
		movlw   0x23		; dir. de la tarjeta donde registrar D_fin 
		movwf	txbuf
		call	TX
		movf	DD,0		; Escribo DD
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing

		call	BSTART
		movlw	0x38
		movwf	txbuf
		call	TX
		movlw	0x24		; dir. de la tarjeta donde registrar M_fin
		movwf	txbuf
		call	TX
		movf	M_fin,0		; Escribo MM + 1		
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing

		call	BSTART
		movlw	0x38
		movwf	txbuf
		call	TX
		movlw	0x25		; dir. de la tarjeta donde registrar A_fin
		movwf	txbuf
		call	TX
		movf	A_fin,0		; Escribo AA modificado
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

		movlw	0x03		; Uso estos registros porque
		movwf	Dir2		; Contador,1,2,3 son usados
		movlw	0x26		; por otras subrutinas
		movwf	Dir1

Siguiente2	call	BSTART
		movlw	0x38		; Mando el comando para escribir
		movwf	txbuf		; en la Main Memory
		call	TX
		movf	Dir1,0		; dir. de la tarjeta a recargar
		movwf	txbuf
		call	TX
		movlw	0xFF		; Escribo 0xFF
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing

		decfsz	Dir2,1		; Cada vez que escribo 3 bytes
		goto	No_Punto	;   pinto un punto en el LCD
		movlw	0x03
		movwf	Dir2
		MOVLW	"."
		CALL	LCD_Dato_P1
		BCF	PCLATH,3	; Selecciona página 0
		CALL	Pausa_500ms
		BSF	PCLATH,3	; Selecciona página 1

No_Punto	incf	Dir1,1
		movlw	0x44		; 0x44 = 0x26 + 0x1E (30 bytes)
		subwf	Dir1,0
		btfss	STATUS,Z
		goto	Siguiente2

		movlw	0x26
		movwf	FSR
		return

;******************************* Subrutina CambiarPIN **********************************
;		            Opción 2 del Menú de Usuario
;***************************************************************************************

CambiarPIN	call	BSTART
		movlw	0x39		; Mando el comando para escribir
		movwf	txbuf		; en la Security Memory
		call	TX
		movlw   0x01		; dir. de la tarjeta donde registrar nuevo PIN1 
		movwf	txbuf
		call	TX
		movf	PIN1,0		; Escribo PIN1
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing
		
		call	BSTART
		movlw	0x39		; Mando el comando para escribir
		movwf	txbuf		; en la Security Memory
		call	TX
		movlw   0x02		; dir. de la tarjeta donde registrar nuevo PIN2
		movwf	txbuf
		call	TX
		movf	PIN2,0		; Escribo PIN2
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing

		call	BSTART
		movlw	0x39		; Mando el comando para escribir
		movwf	txbuf		; en la Security Memory
		call	TX
		movlw   0x03		; dir. de la tarjeta donde registrar nuevo PIN3
		movwf	txbuf
		call	TX
		movf	PIN3,0		; Escribo PIN3
		movwf	txbuf
		call	TX
		call	BSTOP
		call 	Processing

		return

;******************************** Subrutina Processing ********************************
;			 Lectura 32 bytes = 256 pulsos de CLK
;**************************************************************************************

Processing	movlw	0x20		
		movwf	Contador3
PpioProcess	decfsz	Contador3,1
		goto	Process
		return
Process		call	RX
		goto	PpioProcess

;******************************** Subrutina Quita1intento *****************************
;			Pone a 0 uno de los bits a uno del registro EC
;**************************************************************************************

Quita1intento	btfss	EC,0
		goto	Bit1
		bcf	EC,0
		return
Bit1		btfss	EC,1
		goto	Bit2
		bcf	EC,1
		return
Bit2		btfss	EC,2
		return


