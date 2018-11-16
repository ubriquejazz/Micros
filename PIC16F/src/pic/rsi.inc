;***************************************************************************************;
;											;
;					 RSI.inc					;
;											;
;			Rutina de Servicio a la Interrupción TMR0			;
;			Modifica las siguientes variables globales:			;
;	* Contador									;
;	* hh, mm, ss, DD, MM, AA							;
;	* NuevoDia									;
;	* PAGE0, PAGE1									;
;	* w_temp, status_temp								;
;											;
;***************************************************************************************;

RSI		movwf   w_temp          ; save off current W register contents
		movf	STATUS,0        ; move status register into W register
		movwf	status_temp     ; save off contents of STATUS register

		DECFSZ	Contador,1
		GOTO 	Seguir		; Si Contador no es cero
		MOVLW	10h
		MOVWF	Contador

		MOVLW	.59
		SUBWF	ss,0		; ¿ ss = 59 ?
		BTFSS	STATUS,Z
		GOTO 	Inc_ss		;   ss = ss + 1
		CLRF	ss		;   ss = 00

		MOVLW	.59
		SUBWF	mm,0		; ¿ mm = 59 ?
		BTFSS	STATUS,Z
		GOTO 	Inc_mm		;   mm = mm + 1
		CLRF	mm		;   mm = 00
		
		MOVLW	.23
		SUBWF	hh,0		; ¿ hh = 23 ?
		BTFSS	STATUS,Z
		GOTO 	Inc_hh		;   hh = hh + 1
		CLRF	hh		;   hh = 00

;************************************** NUEVO DIA *************************************

		BSF	buffer,NuevoDia	; NuevoDia = 1
		CLRF	VALUE
		CLRF	ADDR
		CALL	Escribir_EE	; Visita = 0
		MOVLW	0x01
		MOVWF	ADDR
		CALL	Escribir_EE
		
		MOVF	MM,0
		CALL	DiasEnMes
		SUBWF	DD,0		; ¿ DD = DiasEnMes ?
		BTFSS	STATUS,Z
		GOTO 	Inc_DD		;   DD = DD + 1

;*********************************** DD = DiasEnMes ************************************

		MOVLW	0x02		; ¿ (MM = 2) y ( DD = 28 ) ?
		SUBWF	MM,0	
		BTFSS	STATUS,Z
		GOTO	Uno_Marzo
		BTFSC	AA,0		; Aquí entra si (MM = 2) y ( DD = 28 )
		GOTO	Uno_Marzo	; por lo que vemos si AA es bisiesto
		BTFSC	AA,1
		GOTO	Uno_Marzo
		GOTO	Inc_DD		; ( AA % 4 = 0 ) y ( MM = 2 ) y ( DD = 28 )
		
Uno_Marzo	MOVLW	01h		; Si AA no es bisiesto, DD = 01
		MOVWF	DD		
		MOVWF	VALUE
		MOVLW	0x06
		MOVWF	ADDR
		CALL	Escribir_EE

		MOVLW	.12		; ¿ MM = 12 ?
		SUBWF	MM,0
		BTFSS	STATUS,Z
		GOTO 	Inc_MM		;   MM = MM + 1
		MOVLW	01h		;   MM = 01
		MOVWF	MM
		MOVWF	VALUE
		MOVLW	0x05
		MOVWF	ADDR
		CALL	Escribir_EE
		
		MOVLW	.99		; ¿ AA = 99 ?
		SUBWF	AA,0
		BTFSS	STATUS,Z
		GOTO 	Inc_AA		;   AA = AA + 1
		CLRF	AA		;   AA = 00
		CLRF	VALUE
		MOVLW	0x04
		MOVWF	ADDR
		CALL	Escribir_EE
		GOTO 	Seguir

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Inc_AA		INCF	AA,0
		MOVWF	AA
		MOVWF	VALUE
		MOVLW	0x04
		MOVWF	ADDR
		CALL	Escribir_EE
		GOTO 	Seguir

Inc_MM		INCF	MM,0
		MOVWF	MM
		MOVWF	VALUE
		MOVLW	0x05
		MOVWF	ADDR
		CALL	Escribir_EE
		GOTO 	Seguir

Inc_DD		MOVLW	0x02		; ¿ ( MM = 2 ) y ( DD = 29 ) ?
		SUBWF	MM,0
		BTFSS	STATUS,Z
		GOTO	No_29_Feb	; Hoy no es 29 de Febrero
		MOVLW 	.29
		SUBWF	DD,0
		BTFSC	STATUS,Z
		GOTO	Uno_Marzo	; Dia siguiente = 1 de Marzo

No_29_Feb	INCF	DD,0
		MOVWF	DD
		MOVWF	VALUE
		MOVLW	0x06
		MOVWF	ADDR
		CALL	Escribir_EE
		GOTO 	Seguir

Inc_hh		INCF	hh,1
		GOTO 	Seguir

Inc_mm		INCF	mm,1
		GOTO 	Seguir

Inc_ss		INCF	ss,1

Seguir		MOVLW	0Ch
		MOVWF	TMR0		; carga TMR0
		BSF	STATUS,RP0
		MOVLW	b'10000111'
		MOVWF	OPTION_REG	; Predivisor 256
		BCF	STATUS,RP0
		MOVLW	b'10100000'
		MOVWF	INTCON		; limpia T0IF y activa GIE y T0IE

		bcf 	PCLATH,3
		btfsc	buffer,PAGE0	; PCLATH(3) = buffer(3)
		bsf	PCLATH,3

		movf    status_temp,0 	; retrieve copy of STATUS register
		movwf	STATUS          ; restore pre-isr STATUS register contents
		swapf   w_temp,1
		swapf   w_temp,0        ; restore pre-isr W register contents
		retfie                  ; return from interrupt

