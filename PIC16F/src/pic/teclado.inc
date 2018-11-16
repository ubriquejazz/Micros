;***************************************************************************************;
;											;
;					TECLADO.inc					;
;											;
;			    Subrutinas para manejar el teclado				;
;											;
;  Modifica las variables globales Tecla, Contador1 y Contador2				;
;											; 
;***************************************************************************************;

Pausa_500ms	CLRF	Contador1
		CLRF	Contador2
Pausa2		DECFSZ 	Contador1,1
		GOTO 	Pausa2
		DECFSZ	Contador2,1
		GOTO	Pausa2
		RETURN

KB_Port		BSF	STATUS,RP0
		MOVLW	b'11110000'
		MOVWF	TRISB
		BCF	OPTION_REG,7	; Habilita las resistencias pull-up
		BCF	STATUS,RP0
		RETURN

KB_Scan		CLRF	Tecla
		INCF	Tecla,1
		MOVLW	0Eh
		MOVWF	PORTB

Cheq_Col	CALL	Pausa_500ms	; Para que se suelte la pulsación anterior
		BTFSS	PORTB,4
		RETURN
		INCF	Tecla,1
		BTFSS	PORTB,5
		RETURN
		INCF	Tecla,1
		BTFSS	PORTB,6
		RETURN
		INCF	Tecla,1
		BTFSS	PORTB,7
		RETURN
		INCF	Tecla,1

Ultima_Tecla	MOVLW	0x011		; ¿ Tecla = 17 ?
		SUBWF	Tecla,0
		BTFSC	STATUS,Z
		GOTO 	NTeclas
		BSF	STATUS,C
		RLF	PORTB,1
		GOTO	Cheq_Col

NTeclas		CLRF	Tecla
		RETURN
