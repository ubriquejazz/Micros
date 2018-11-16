;***************************************************************************************;
;											;
;					TRJ_USER.inc					;
;											;
;            Librería con subrutinas que hacen de interfaz entre usuario y tarjetas	;
;											;
;	     Modifica la variable global max						;
;											;
;***************************************************************************************;

;********************************** Subrutina Mostrar_EC ******************************
;											
;	Muestra "PIN(error=x):" donde x es la cantidad de errores que 			
;	se han cometido hasta el momento en la introducción del PIN. 			
;	Si se llega al 3º error, no se puedrá escribir en la tarjeta .			
;										
;**************************************************************************************
 
Mostrar_EC	CLRF	Contador3

Coge_Letra40	MOVF	Contador3,0
		CALL	PIN_error	; "PIN(error="
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje40
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra40

FinMensaje40	movf	EC,0		
		bcf	PCLATH,3	; selecciona página 0
		call	ConvierteEC	; cuenta el nº de 0 en el registro EC
		bsf	PCLATH,3	; selecciona página 1
		addlw	0x30
		call	LCD_Dato_P1
		call	Pausa_5ms_P1

		MOVLW	")"
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		MOVLW	":"
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		RETURN

;*********************************** Subrutina TeclearPIN *****************************
;
;	El PIN tecleado se cifra con el PIN maestro (según el algoritmo del Cesar) 
; 	antes de ser comparado con el PSC de la tarjeta
;
; OJO: 	MASTER_PINx almacena el nº de tecla, luego antes de nada ha de ser convertido 
;	al dígito correspondiente.
;
;**************************************************************************************

TeclearPIN	MOVLW	CursBlink	; Parpadeo del cursor
		CALL	LCD_Comando_P1
		CALL	Pausa_5ms_P1

ScanPIN1	bcf	PCLATH,3	; selecciona página 0
		CALL	KB_Port
		CALL	KB_Scan
		bsf	PCLATH,3	; selecciona página 1
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	ScanPIN1
		CALL	LCD_Port_P1
		MOVLW	0x05
		SUBWF	Tecla,0		; Tecla < 5  no
		BTFSS	STATUS,C	; estará permitido
		GOTO	ScanPIN1

		MOVF	Tecla,0
		bcf	PCLATH,3	; selecciona página 0
		CALL	Conversion
		bsf	PCLATH,3	; selecciona página 1
		MOVWF	PIN1
		ADDLW	30h
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1

		MOVF	MASTER_PIN1,0	; PIN1 = (PIN1 + MASTER_PIN1) % 10
		bcf	PCLATH,3	; selecciona página 0
		CALL	Conversion
		bsf	PCLATH,3	; selecciona página 1
		ADDWF 	PIN1,0		
		BCF	PCLATH,3	; selecciona página 0
		CALL	Bin8BCD
		BSF	PCLATH,3	; selecciona página 1
		MOVF 	Unidades,0
		MOVWF	PIN1		

ScanPIN2	bcf	PCLATH,3	; selecciona página 0
		CALL	KB_Port
		CALL	KB_Scan
		bsf	PCLATH,3	; selecciona página 1
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	ScanPIN2
		CALL	LCD_Port_P1
		MOVLW	0x05
		SUBWF	Tecla,0		; Tecla < 5  no
		BTFSS	STATUS,C	; estará permitido
		GOTO	ScanPIN2

		MOVF	Tecla,0
		bcf	PCLATH,3	; selecciona página 0
		CALL	Conversion
		bsf	PCLATH,3	; selecciona página 1
		MOVWF	PIN2
		ADDLW	30h
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1

		MOVF	MASTER_PIN2,0	; PIN2 = (PIN2 + MASTER_PIN2) % 10
		bcf	PCLATH,3	; selecciona página 0
		CALL	Conversion
		bsf	PCLATH,3	; selecciona página 1
		ADDWF 	PIN2,0
		BCF	PCLATH,3	; selecciona página 0
		CALL	Bin8BCD
		BSF	PCLATH,3	; selecciona página 1
		MOVF 	Unidades,0
		MOVWF	PIN2		

ScanPIN3	bcf	PCLATH,3	; selecciona página 0
		CALL	KB_Port
		CALL	KB_Scan
		bsf	PCLATH,3	; selecciona página 1
		MOVF	Tecla,0
		BTFSC	STATUS,Z
		GOTO	ScanPIN3
		CALL	LCD_Port_P1
		MOVLW	0x05
		SUBWF	Tecla,0		; Tecla < 5  no
		BTFSS	STATUS,C	; estará permitido
		GOTO	ScanPIN3

		MOVF	Tecla,0
		bcf	PCLATH,3	; selecciona página 0
		CALL	Conversion
		bsf	PCLATH,3	; selecciona página 1
		MOVWF	PIN3
		ADDLW	30h
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1

		MOVF	MASTER_PIN3,0	; PIN3 = (PIN3 + MASTER_PIN3) % 10
		bcf	PCLATH,3	; selecciona página 0
		CALL	Conversion
		bsf	PCLATH,3	; selecciona página 1
		ADDWF 	PIN3,0
		BCF	PCLATH,3	; selecciona página 0
		CALL	Bin8BCD
		BSF	PCLATH,3	; selecciona página 1
		MOVF 	Unidades,0
		MOVWF	PIN3		

		MOVLW	CursOff		
		CALL	LCD_Comando_P1
		CALL	Pausa_5ms_P1	; Fin del parpadeo del cursor
		RETURN

;******************************** Subrutina ModoEmpleo ********************************
;
;		    Muestra "* Menu Usuario */n Pulse 1,2,3 o 4."
;
;**************************************************************************************

ModoEmpleo	MOVLW	LCDCLR		; Borra pantalla, cursor a casa
		CALL	LCD_Comando_P1
		CALL	Pausa_5ms_P1

		MOVLW	"*"
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		CLRF	Contador3
Coge_Letra41	MOVF	Contador3,0
		bcf	PCLATH,3	; selecciona página 0
		CALL	MenuUsuario	; " Menu Usuario "
		bsf	PCLATH,3	; selecciona página 1
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje41
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra41
FinMensaje41	MOVLW	"*"
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1

		MOVLW	LCDLinea2	; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P1
		CALL	Pausa_5ms_P1

		CLRF	Contador3
Coge_Letra42	MOVF	Contador3,0
		bcf	PCLATH,3	; selecciona página 0
		CALL	Pulse		; "Pulse 1,2,3 o 4."
		bsf	PCLATH,3	; selecciona página 1
		IORLW	00h
		BTFSC	STATUS,Z
		RETURN		
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra42

;******************************** Subrutina Pantallazo ********************************
;
;	Muestra la pareja de opciones actual determinada por el registro Pareja
;
;**************************************************************************************

Pantallazo	MOVLW	LCDCLR		; Borra pantalla, cursor a casa
		CALL	LCD_Comando_P1
		CALL	Pausa_5ms_P1

Fila0		MOVLW	LCDLinea1	; Posiciona el cursor en la 1ª línea 
		CALL	LCD_Comando_P1
		CALL	Pausa_5ms_P1

Pareja0Fila0?	MOVF	Pareja,0	; ¿ Pareja = 0 ?
		BTFSS	STATUS,Z
		GOTO	Pareja1Fila0
		CLRF	Contador3
		BTFSC	buffer,TU	; Si TU = 0, "1. Entrar"
		GOTO	Coge_Letra44	; Si TU = 1, "1. Recargar"	

Coge_Letra43	MOVF	Contador3,0
		CALL	Entrar		; "1. Entrar"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	Fila1
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra43

Coge_Letra44	MOVF	Contador3,0
		CALL	Recargar	; "1. Recargar"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	Fila1
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra44

Pareja1Fila0	CLRF	Contador3	; Pareja = 1
Coge_Letra45	MOVF	Contador3,0
		CALL	Ver		; "3. Ver Saldo"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	Fila1
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra45

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Fila1		MOVLW	LCDLinea2	; Posiciona el cursor en la 2ª línea 
		CALL	LCD_Comando_P1
		CALL	Pausa_5ms_P1

Pareja0Fila1?	MOVF	Pareja,0	; ¿ Pareja = 0 ?
		BTFSS	STATUS,Z
		GOTO	Pareja1Fila1
		CLRF	Contador3
Coge_Letra46	MOVF	Contador3,0
		CALL	Cambiar		; "2. Cambiar PIN"
		IORLW	00h
		BTFSC	STATUS,Z
		RETURN
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra46

Pareja1Fila1	CLRF	Contador3	; Pareja = 2
Coge_Letra47	MOVF	Contador3,0
		CALL	Abandonar	; "4. Abandonar"
		IORLW	00h
		BTFSC	STATUS,Z
		RETURN
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra47

;****************************** Subrutina Escanea3seg *********************************
;
;		Escanea el teclado hasta que se pulse o pasen 3 segundos
;
;**************************************************************************************

Escanea3seg	bcf	PCLATH,3	; selecciona página 0
		call	KB_Port
		bsf	PCLATH,3	; selecciona página 1

		movf	ss,0
		addlw	0x03
		movwf	max		; max = ss + 3
		movlw 	.60
		subwf 	max,0		; ¿ max < 60 ?	
		btfss	STATUS,C
		goto	EscaneaUsuario	; Si max < 60, max es correcto
		movlw	.60 		
		subwf	max,1		; Si no, max = max - 60

EscaneaUsuario	BCF	PCLATH,3	; selecciona página 0
		CALL	KB_Scan
		BSF	PCLATH,3	; selecciona página 1
		MOVF	Tecla,0
		BTFSS	STATUS,Z	; Mientras (Tecla = 0)
		RETURN
		MOVF	ss,0		;          &
		SUBWF	max,0
		BTFSC	STATUS,Z	;     (ss != max)
		RETURN
		GOTO	EscaneaUsuario	;    EscaneaUsuario

;********************************** Subrutina Pintar_Lleno ****************************
;
;       Muestra "Tiene 10 tickets" en el caso particular de que el bono esté lleno
;
;**************************************************************************************

Pintar_Lleno	CLRF	Contador3

Coge_Letra48	MOVF	Contador3,0
		CALL	Tiene		; "Tiene_"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje48
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra48

FinMensaje48	MOVLW	"1"
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		MOVLW	"0"
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1

		CLRF	Contador3
Coge_Letra49	MOVF	Contador3,0
		CALL	tickets		; "_tickets"
		IORLW	00h
		BTFSC	STATUS,Z
		RETURN
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra49

;********************************* Subrutina Ver_Saldo ********************************
;			      Opción 3 del Menú de Usuario
;**************************************************************************************

VerSaldo	CLRF Contador3

Coge_Letra4A	MOVF	Contador3,0
		CALL	Tiene		; "Tiene "
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje4A
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra4A

FinMensaje4A	MOVF	Saldo,0		; Escribe Saldo en el LCD
		ADDLW	0x30
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		
		CLRF	Contador3
		BTFSC	buffer,Cobrar	; Si no Cobrar, muestra 
		GOTO	Coge_Letra4B	; el asterisco indicativo
		MOVLW	"*"
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1

Coge_Letra4B	MOVF	Contador3,0
		CALL	tickets		; "_tickets"
		IORLW	00h
		BTFSC	STATUS,Z
		RETURN
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra4B

;******************************* Subrutina Mostrar_Saldo ******************************
;
;   Tras pulsar la opción "Entrar" del Menú de Usuario, muestra el saldo que queda
;
;**************************************************************************************

Mostrar_Saldo	CLRF	Contador3

Coge_Letra4C	MOVF	Contador3,0
		CALL	Quedan		; "Quedan_"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje4C
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra4C

FinMensaje4C	MOVF	Saldo,0
		ADDLW	0x30
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1

		CLRF	Contador3
Coge_Letra4D	MOVF	Contador3,0
		CALL	tickets		; "_tickets"
		IORLW	00h
		BTFSC	STATUS,Z
		RETURN
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra4D

;********************************* Subrutina Inc_Visita *******************************
;
;	     Tras pulsar la opción "Entrar" del Menú de Usuario, incrementa  
;         el número de visitantes en el caso de que haya que cobrar (Cobrar = 1)
;
;**************************************************************************************

Inc_Visita	BCF	PCLATH,3	; selecciona página 0

		MOVLW	0x00
		CALL	Leer_EE	
		MOVWF	L_byte		; L_byte = EE(0), byte bajo de VISITA
		MOVLW	0x01		; H_byte = EE(1), byte alto de VISITA
		CALL	Leer_EE	
		MOVWF	H_byte

		BCF	STATUS,Z
		INCF	L_byte,1
		BTFSC	STATUS,Z	; Si L_byte era 0xFF
		INCF	H_byte,1	;  incrementa H_byte

		CLRF	ADDR
		MOVF	L_byte,0
		MOVWF	VALUE
		CALL	Escribir_EE	; EE(0) = L_byte, byte bajo de VISITA
		INCF	ADDR,1
		MOVF	H_byte,0
		MOVWF	VALUE
		CALL	Escribir_EE	; EE(1) = H_byte, byte alto de VISITA

		BSF	PCLATH,3	; selecciona página 1
		RETURN


