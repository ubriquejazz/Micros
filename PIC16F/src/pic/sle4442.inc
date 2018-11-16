;***************************************************************************************;
;											;
;					SLE4442.inc					;
;											;
;	     Esta librería convierte los bytes procedentes de TRJT_CMD.inc en un 	;
;	   tren de pulsos adecuado para la tarjeta con chip SLE4442 y viceversa. 	;
;	     La sincronización se ha sacado de los cronogramas de su datasheet.		;
;											;
;	Usa las variables datai, datao, di, do						;
;	Modifica la variable global Contador1						;
;											;
;***************************************************************************************;

;********************************* Subrutina PulsoRST *********************************

PulsoRST	BSF	PORTC,RST
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		BSF	PORTC,CLK
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		BCF	PORTC,CLK
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		BCF	PORTC,RST
		RETURN

;********************************** Subrutina BSTART **********************************

BSTART		bsf	STATUS,RP0
		bcf	TRISC,IO
		bcf	STATUS,RP0
		bsf	PORTC,IO
		bcf	PORTC,CLK
		nop
		nop
		nop
		bsf	PORTC,CLK
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		bcf	PORTC,IO
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		bcf	PORTC,CLK
		nop
		nop
		return

;*********************************** Subrutina BSTOP **********************************

BSTOP		bsf	STATUS,RP0
		bcf	TRISC,IO
		bcf	STATUS,RP0
		bcf	PORTC,IO
		nop
		nop
		nop
		bsf	PORTC,CLK
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		bsf	PORTC,IO
		nop
		nop
		bcf	PORTC,CLK
		nop
		nop
		return		

;********************************** Subrutina BITIN ***********************************

BITIN		bsf	buffer,di
		bsf	STATUS,RP0
		bsf	TRISC,IO
		bcf	STATUS,RP0
		bsf	PORTC,CLK
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		btfss	PORTC,IO
		bcf	buffer,di
		bcf	PORTC,CLK
		return

;********************************* Subrutina BITOUT ***********************************

BITOUT		bsf	STATUS,RP0
		bcf	TRISC,IO
		bcf	STATUS,RP0
		btfss	buffer,do
		goto	bitlow
		bsf	PORTC,IO
		goto	clkout

bitlow		bcf	PORTC,IO
clkout		bsf	PORTC,CLK
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		bcf	PORTC,CLK
		return

;************************************ Subrutina RX ************************************

RX		movlw	0x08
		movwf	Contador1
		clrf	datai
		bcf	STATUS,C
RXLP		rrf	datai,1
		call	BITIN
		btfsc	buffer,di
		bsf	datai,7
		decfsz	Contador1,1
		goto 	RXLP
		return

;************************************ Subrutina TX ************************************

TX		movlw	0x08
		movwf	Contador1

TXLP		bcf	buffer,do
		btfsc	txbuf,0
		bsf	buffer,do
		call	BITOUT
		rrf	txbuf,1
		decfsz	Contador1,1
		goto 	TXLP
		return

