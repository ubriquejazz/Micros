;***************************************************************************************;
;											;
;					TARJETA.inc					;
;											;
;	Subrutina que gestiona las tarjetas con chips introducidas por el usuario	;
;											;
;											;
;***************************************************************************************;

Tarjeta		call	PulsoRST	; Tras el reset se recibe 
					; el Manufacter Code
		movlw	LCDCLR	
		call	LCD_Comando_P1
		call	Pausa_5ms_P1	; Borra pantalla, cursor a casa

;********************************* ¿ TARJETA ERRÓNEA ? ********************************

		call	RX		; Si los 4 bytes que se reciben son
		movlw	0x0A2		; 0xA2, 0x13, 0x10 y 0x91 se trata
		subwf	datai,0		; de una tarjeta SLE4442 (válida 
		btfss	STATUS,Z	; para nuestra aplicación).
		goto	Erronea		; Si no, la tarjeta es errónea

		call	RX		
		movlw	0x013	
		subwf	datai,0
		btfss	STATUS,Z
		goto	Erronea
		
		call	RX
		movlw	0x010	
		subwf	datai,0
		btfss	STATUS,Z
		goto	Erronea

		call	RX		
		movlw	0x091
		subwf	datai,0
		btfss	STATUS,Z
		goto	Erronea

;*********************************** ¿ VISITA = AFORO ? *******************************

		BCF	PCLATH,3	; selecciona página 0
		MOVLW	0x00
		CALL	Leer_EE	
		MOVWF	L_byte		; L_byte = EE(0), byte bajo de VISITA
		MOVLW	0x01		; H_byte = EE(1), byte alto de VISITA
		CALL	Leer_EE	
		MOVWF	H_byte
		MOVLW	0x02	
		CALL	Leer_EE		; L_temp = EE(2), byte bajo de AFORO MAXIMO
		MOVWF	L_temp		; H_temp = EE(3), byte alto de AFORO MAXIMO
		MOVLW	0x03	
		CALL	Leer_EE	
		MOVWF	H_temp
		BSF	PCLATH,3	; selecciona página 1

		MOVF	L_temp,0	; Si byte bajo de AFORO   distinto
		SUBWF	L_byte,0	; de byte bajo de VISITA, entonces 
		BTFSS	STATUS,Z	; aforo no completo
		GOTO	A_No_Completo
					; Aqui los bytes bajos son iguales 
		MOVF	H_temp,0	; Si byte alto de AFORO   distinto
		SUBWF	H_byte,0	; de byte alto de VISITA, entonces  
		BTFSS	STATUS,Z	; aforo no completo
		GOTO	A_No_Completo
		GOTO	A_Completo	; Aqui VISITA = AFORO

;******************************* RAM(20:43) = MM(20:43) *******************************

A_No_Completo	call	BSTART		; Mando comando de lectura de la Main
		movlw	0x30		; Memory a partir de la dirección 0x20
		movwf	txbuf
		call	TX
		movlw	0x20
		movwf	txbuf
		call	TX
		movlw	0x00
		movwf	txbuf
		call	TX
		call	BSTOP

		movlw	0x20		; Cargo los 36 bytes útiles a partir 
		movwf	FSR		; de la dirección 0x20 de la RAM
Siguiente1	call	RX
		movf	datai,0
		movwf	INDF
		incf	FSR,1
		movlw	0x44		; 0x44 = 0x20 + 0x24 (36 bytes)
		subwf	FSR,0
		btfss	STATUS,Z
		goto	Siguiente1

		movlw	0xBD		; Leo el resto de los 189 bytes inútiles
		movwf	Contador3	; pero no los guardo ( se pierden )
PpioInutil	decfsz	Contador3,1
		goto	Inutil
		goto	Validez?
Inutil		call	RX
		goto	PpioInutil

;************************************ ¿ VALIDEZ ? *************************************

Validez?	movlw 	0x20		; Deduzco si estamos dentro del
		movwf	FSR		; período de validez del bono 
		movf	INDF,0
		movwf	D_ini		; Guardo fecha de inicio de validez
		incf	FSR,1
		movf	INDF,0
		movwf	M_ini
		incf	FSR,1
		movf	INDF,0
		movwf	A_ini
		incf	FSR,1		; Guardo fecha en que caduca el bono
		movf	INDF,0
		movwf	D_fin
		incf	FSR,1
		movf	INDF,0
		movwf	M_fin
		incf	FSR,1
		movf	INDF,0
		movwf	A_fin

Pronto?		movf	A_ini,0
		subwf	AA,0
		btfss	STATUS,C
		goto 	AunNoValido	; Si AA < A_ini, todavía no es la  
					; fecha en la que es válido el bono
		movf	AA,0
		subwf	A_ini,0
		btfss	STATUS,Z	; ¿ AA = A_ini ?
		goto	Tarde?

		movf	M_ini,0		; Aquí AA = A_ini
		subwf	MM,0
		btfss	STATUS,C
		goto 	AunNoValido	; Si MM < M_ini, aun no es la fecha

		movf	MM,0
		subwf	M_ini,0		; ¿ MM = M_ini ?
		btfss	STATUS,Z
		goto	Tarde?

		movf	D_ini,0		; Aquí AA = A_ini, MM = M_ini
		subwf	DD,0
		btfss	STATUS,C
		goto 	AunNoValido	; Si DD < D_ini, aun no es la fecha

Tarde?		movf	AA,0
		subwf	A_fin,0
		btfss	STATUS,C
		goto 	Caducado	; Si A_fin < AA, el bono está caducado

		movf	AA,0
		subwf	A_fin,0		; ¿ AA = A_fin ?
		btfss	STATUS,Z
		goto	No_Caducado

		movf	MM,0		; Aquí AA = A_fin
		subwf	M_fin,0
		btfss	STATUS,C
		goto 	Caducado	; Si M_fin < MM, el bono está caducado

		movf	MM,0
		subwf	M_fin,0		; ¿ MM = M_fin ?
		btfss	STATUS,Z
		goto	No_Caducado

		movf	DD,0		; Aquí AA = A_fin, MM = M_fin
		subwf	D_fin,0
		btfss	STATUS,C
		goto 	Caducado	; Si D_fin < DD, el bono está caducado

;************************************* ¿ LLENO ? **************************************

No_Caducado	movlw	0x0A
		movwf	Saldo		; Saldo = 10
		bsf	buffer,Cobrar	; Cobrar = 1
		movlw 	0x26		; Veo si se  ha hecho 
		movwf	FSR		; al menos un ticket
		movlw	0xFF
		subwf	INDF,0
		btfsc	STATUS,Z
		goto	Lleno		; Si no, el bono está lleno

;************************************ SI NO LLENO *************************************

Mientras	movlw	0xFF		; Bucle para leer el saldo
		subwf	INDF,0
		btfsc	STATUS,Z
		goto	FinMientras
		movlw	0x44		; Comparo si he llegado a la 
		subwf	FSR,0		; última posición de la tabla
		btfsc	STATUS,Z
		goto	FinMientras
		decf	Saldo,1
		movlw	0x03
		addwf	FSR,1
		goto	Mientras	; Saldo leído

FinMientras	CLRF	Contador3

Coge_Letra30	MOVF	Contador3,0
		CALL	Tiene		; "Tiene "
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje30
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra30

FinMensaje30	MOVF	Saldo,0		; Escribe Saldo en el LCD
		ADDLW	0x30
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1

;********************************** ¿ HOY = ULTIMO DIA ? ******************************

		movlw	0x03		; Guardo el día en que se realizó
		subwf	FSR,1		; la última entrada al recinto
		movf	INDF,0
		movwf	D_ult
		incf	FSR,1
		movf	INDF,0
		movwf	M_ult
		incf	FSR,1
		movf	INDF,0
		movwf	A_ult		; FSR = dirección donde registrar
		incf	FSR,1		; la nueva fecha en la tarjeta

		movf	DD,0		; ¿ Hoy es el día de la última entrada ?
		subwf	D_ult,0
		btfss	STATUS,Z	; ¿ DD = D_ult ?
		goto	Otro_Dia
		movf	MM,0
		subwf	M_ult,0
		btfss	STATUS,Z	; ¿ MM = M_ult ?
		goto	Otro_Dia
		movf	AA,0
		subwf	A_ult,0
		btfss	STATUS,Z	; ¿ AA = A_ult ?
		goto	Otro_Dia

Mismo_Dia	MOVLW	"*"
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		bcf	buffer,Cobrar	; Cobrar = 0

;***************************************** OTRO DIA ***********************************

Otro_Dia	CLRF	Contador3
Coge_Letra31	MOVF	Contador3,0
		CALL	tickets		; "_tickets"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje31
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra31

FinMensaje31	movf	Saldo,1		; ¿ Bono vacío & Cobrar ?
		btfss	STATUS,Z
		goto	No_Vacio
		btfss	buffer,Cobrar
		goto 	No_Vacio
Vacio		bsf	buffer,TU	; TU = 1
		goto	Menu_Usuario

No_Vacio	bcf	buffer,TU	; TU = 0
		goto	Menu_Usuario

;*********************************** BONO CADUCADO ************************************

Caducado	CLRF	Saldo		; Saldo = 0
		CLRF	Contador3
Coge_Letra32	MOVF	Contador3,0
		CALL	BonoCaducado	; " Bono caducado"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje32
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra32
FinMensaje32	BSF	buffer,TU	; TU = 1
		GOTO	Menu_Usuario

;************************************ BONO LLENO **************************************

Lleno		CALL	Pintar_Lleno
		BCF	buffer,TU	; TU = 0
		GOTO	Menu_Usuario

;**************************************************************************************
;******************************** Sección Menu_Usuario ********************************
;**************************************************************************************

Menu_Usuario	CALL	LeerEC

Intento		CLRF	Contador3
		MOVLW	LCDLinea2	; Posiciona el cursor en la 2ª línea
		CALL	LCD_Comando_P1
		CALL	Pausa_5ms_P1

		call 	Mostrar_EC	; Muestra el nº de PINs erróneos que 
		call	TeclearPIN	; se han introducido hasta el momento

		btfss	PORTC,CARD	; Antes de comparar el PIN tecleado
		goto 	FinTarjeta	; compruebo si el bono sigue insertado
		call	CompPSC	
		call	LeerEC		; Compruebo si el PIN es correcto
		movlw	0x07
		subwf	EC,0
		btfss	STATUS,Z	
		goto	Intento		; Si no coinciden, gasto un intento
 
		bcf	PORTC,LEDR	; LEDR  = 0 (verde)
		bcf	PCLATH,3
		bcf 	PORTC,ZUMB	; ZUMB = 0 (sonido)
		call	Pausa_500ms
		call	Pausa_500ms
		bsf 	PORTC,ZUMB	; ZUMB = 1 (silencio)
		call	Pausa_500ms
		bcf 	PORTC,ZUMB	; ZUMB = 0 (sonido)
		call	Pausa_500ms
		call	Pausa_500ms
		bsf 	PORTC,ZUMB	; ZUMB = 1 (silencio)
		bsf	PCLATH,3
		
		CALL	ModoEmpleo
		BCF	PCLATH,3	; selecciona página 0
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		BSF	PCLATH,3	; selecciona página 1

		CLRF	Pareja		; Pareja = 0

PintaUsuario	CALL	Pantallazo	; Muestra la pareja 					
		INCF	Pareja,1	; de opciones actual
		MOVLW	0x02
		SUBWF	Pareja,0	; Pareja = Pareja + 1
		BTFSS	STATUS,Z
		GOTO	Pareja_no_2	; Si Pareja = 2,	
		CLRF	Pareja		;    Pareja = 0

Pareja_no_2	CALL 	Escanea3seg	; Escanea hasta que se pulse
		CALL	LCD_Port_P1	; o que pasan 3 segundos
	
		MOVLW	LCDCLR		; Borra pantalla, cursor a casa
		CALL	LCD_Comando_P1
		CALL	Pausa_5ms_P1

		BTFSS	PORTC,CARD	; Compruebo si el
		GOTO	FinTarjeta	; bono sigue insertado

		MOVF	Tecla,0
		bcf	PCLATH,3	; selecciona página 0
		CALL	Conversion
		bsf	PCLATH,3	; selecciona página 1
		MOVWF	Tecla

Pulsado_1?	MOVLW	0x01
		SUBWF	Tecla,0		; ¿ Tecla = 1 ?
		BTFSS	STATUS,Z
		GOTO	Pulsado_2?
		btfsc	buffer,TU	; Si TU = 1, Ejecuta_Recarga
		goto	Ejecuta_Recarga 

;****************************************  ENTRAR  ************************************

		btfss	buffer,Cobrar	; Si Cobrar = 1, decrementa 
		goto	No_Cobrar	; Saldo y escribe la fecha
		decf	Saldo,1		; de hoy en la tarjeta.
		call	Escribe_Hoy

No_Cobrar	call	Mostrar_Saldo
		bsf	PORTC,RELE	; RELE = 1 (abrir)

		btfsc	buffer,Cobrar	; Si Cobrar = 0, no
		call	Inc_Visita	; incrementa Visita

		bcf	PCLATH,3	; selecciona página 0
		bcf 	PORTC,ZUMB	; ZUMB = 0 (sonido)
		call	Pausa_500ms
		call	Pausa_500ms
		bsf 	PORTC,ZUMB	; ZUMB = 1 (silencio)
		call	Pausa_500ms
		bcf 	PORTC,ZUMB	; ZUMB = 0 (sonido)
		call	Pausa_500ms
		call	Pausa_500ms
		bsf 	PORTC,ZUMB	; ZUMB = 1 (silencio)
		bsf	PCLATH,3	; selecciona página 1
		goto	Salida

;**************************************** RECARGAR ************************************

Ejecuta_Recarga	CLRF	Contador3

Coge_Letra33	MOVF	Contador3,0
		CALL	InserteDinero	; " Inserte dinero"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje33
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra33

FinMensaje33	MOVLW	LCDLinea2	; Posiciona el cursor en la 2ª línea
		CALL	LCD_Comando_P1
		CALL	Pausa_5ms_P1
		CLRF	Contador3
Coge_Letra34	MOVF	Contador3,0
		CALL	Validez1mes	; "(validez un mes)"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	EsperaDinero
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra34
		MOVLW	0x01
		MOVWF	Contador4	; i = 1

EsperaDinero	BCF	PCLATH,3
		CALL 	EscaneaCodigo	; Escanea hasta que se pulse
		CALL	LCD_Port_P0	;    o que pase 1 segundo
		BSF	PCLATH,3

RCRG_PIN1?	MOVLW	0x01		;      Si (i = 1)
		SUBWF	Contador4,0
		BTFSS	STATUS,Z	;          &		
		GOTO	RCRG_PIN2?
		MOVF	Tecla,0		; ( Tecla = MASTER_PIN1),
		SUBWF	MASTER_PIN1,0	
		BTFSS	STATUS,Z
		GOTO	Pulsacion?
		INCF	Contador4,1	;       i = i + 1
		GOTO	EsperaDinero

RCRG_PIN2?	MOVLW	0x02		;       Si (i = 2)
		SUBWF	Contador4,0
		BTFSS	STATUS,Z	;           &		
		GOTO	RCRG_PIN3?
		MOVF	Tecla,0		; ( Tecla = MASTER_PIN2),
		SUBWF	MASTER_PIN2,0	
		BTFSS	STATUS,Z
		GOTO	Pulsacion?
		INCF	Contador4,1	;       i = i + 1
		GOTO	EsperaDinero

RCRG_PIN3?	MOVLW	0x03		;       Si (i = 3)
		SUBWF	Contador4,0
		BTFSS	STATUS,Z	;           &		
		GOTO	Error_RCRG
		MOVF	Tecla,0		; ( Tecla = MASTER_PIN3),
		SUBWF	MASTER_PIN3,0	
		BTFSS	STATUS,Z
		GOTO	Pulsacion?
		GOTO 	DineroIn

Pulsacion?	MOVF	Tecla,0		; Si no ha pasado 1 seg.
		BTFSC	STATUS,Z	; pero se ha pulsado algo,
		GOTO	EsperaDinero
Error_RCRG	MOVLW	0x01		; i = 1
		MOVWF	Contador4
		GOTO 	EsperaDinero

DineroIn	BTFSS	PORTC,CARD	; Antes y después de recargar 
		GOTO	FinTarjeta	; veo si el bono sigue insertado
		CALL	RecargarSaldo
		BTFSS	PORTC,CARD
		GOTO	FinTarjeta

		MOVLW	0x0A
		MOVWF	Saldo		; Saldo  = 10
		BSF	buffer,Cobrar	; Cobrar = 1
		BCF	buffer,TU	; TU 	 = 0
		GOTO	PintaUsuario

;************************************* CAMBIAR PIN ************************************

Pulsado_2?	MOVLW	0x02		; ¿ Tecla = 2 ?
		SUBWF	Tecla,0
		BTFSS	STATUS,Z
		GOTO	Pulsado_3?

		CLRF	Contador3
Coge_Letra35	MOVF	Contador3,0
		CALL	Nuevo_PIN	; " Nuevo PIN: "
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje35
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra35

FinMensaje35	CALL	TeclearPIN
		BTFSS	PORTC,CARD	; Antes de cambiar PIN compruebo
		GOTO	FinTarjeta	; si el bono sigue insertado

		CALL	CambiarPIN
		GOTO	PintaUsuario
		
;*************************************** VER SALDO ************************************

Pulsado_3?	MOVLW	0x03		; ¿ Tecla = 3 ?
		SUBWF	Tecla,0
		BTFSS	STATUS,Z
		GOTO	Pulsado_4?

		MOVLW	0x0A
		SUBWF	Saldo,0		; Si Saldo = 10, mostrar
		BTFSS	STATUS,Z	;   "Tiene 10 tickets"
		GOTO	VerNoLleno
		CALL	Pintar_Lleno	
		GOTO	$+2
VerNoLleno	CALL	VerSaldo

		BCF	PCLATH,3	; selecciona página 0
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		CALL	Pausa_500ms
		BSF	PCLATH,3	; selecciona página 1
		GOTO	PintaUsuario

;*************************************** ABANDONAR ************************************

Pulsado_4?	MOVLW	0x04		; ¿ Tecla = 4 ?
		SUBWF	Tecla,0
		BTFSS	STATUS,Z
		GOTO	PintaUsuario

		MOVLW	"F"
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		MOVLW	"i"
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		MOVLW	"n"
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		CLRF	Contador3
Coge_Letra36	MOVF	Contador3,0
		bcf	PCLATH,3	; selecciona página 0
		CALL	MenuUsuario	; " Menu Usuario "
		bsf	PCLATH,3	; selecciona página 1
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	Salida
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra36

;**************************************************************************************
;******************************    FIN  Menu_Usuario    *******************************
;**************************************************************************************

;******************************** BONO AUN NO VÁLIDO **********************************

AunNoValido	BCF	PORTC,ZUMB	; ZUMB = 0 (pitido)
		CLRF	Contador3
Coge_Letra37	MOVF	Contador3,0
		CALL	BaunNoValido	; "B. aun no valido"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	Salida
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra37

;*********************************** AFORO COMPLETO ***********************************

A_Completo	BCF	PORTC,ZUMB	; ZUMB = 0 (pitido)
		CLRF	Contador3	
Coge_Letra38	MOVF	Contador3,0
		CALL	AforoCompleto	; "Aforo Completo"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	Salida
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra38

;*********************************** TARJETA ERRÓNEA **********************************

Erronea		BCF	PORTC,ZUMB	; ZUMB = 0 (pitido)
		MOVLW	"T"
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1	; "T"
		MOVLW	0x01
		MOVWF	Contador3

Coge_Letra39	MOVF	Contador3,0
		CALL	tarjeta		; "arjeta"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje39
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra39

FinMensaje39	CLRF	Contador3
Coge_Letra3A	MOVF	Contador3,0
		CALL	erronea		; " erronea."
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	Salida
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra3A

;********************************** EXTRAIGA TARJETA **********************************

Salida		MOVLW	LCDLinea2	; Posiciona el cursor en la 2ª línea
		CALL	LCD_Comando_P1
		CALL	Pausa_5ms_P1

		CLRF	Contador3
Coge_Letra3B	MOVF	Contador3,0
		CALL	extraiga	; "Extraiga "
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje3B
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra3B

FinMensaje3B	CLRF	Contador3
Coge_Letra3C	MOVF	Contador3,0
		CALL	tarjeta		; "tarjeta"
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	BucleSalida
		CALL	LCD_Dato_P1
		CALL	Pausa_5ms_P1
		INCF	Contador3,1
		GOTO	Coge_Letra3C

BucleSalida	BTFSS	PORTC,CARD	; Espera a que se extraiga la tarjeta
		GOTO	FinTarjeta
		GOTO	BucleSalida

FinTarjeta	BSF	PORTC,ZUMB	; ZUMB = 1 (silencio)
		BCF	PORTC,RELE	; RELE = 0 (para asegurarme)
		BSF	PORTC,LEDR	; LEDR = 1
		MOVLW	LCDCLR		
		CALL	LCD_Comando_P1	; Borra pantalla, cursor a casa
		CALL	Pausa_5ms_P1

		RETURN
