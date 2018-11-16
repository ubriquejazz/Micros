;***************************************************************************************;
;											;
;					EEPROM.inc					;
;											;
;	Usa las variables ADDR y VALUE							;
;											;
;***************************************************************************************;

;******************************** Subrutina Leer_EE ***********************************
;
;	Devuelve en W el contenido de la dirección de la EEPROM pasada en W
;
;**************************************************************************************

Leer_EE		bsf	STATUS,RP1	; Banco 2
		bcf	STATUS,RP0
		movwf	EEADR		; EEADR = W
		bsf	STATUS,RP0	; Banco 3
		bcf	EECON1,EEPGD	; Selecciona Memoria de Datos
		bsf	EECON1,RD	; Empieza la lectura
		bcf	STATUS,RP0	; Banco 2
		movf	EEDATA,0	; W = EEDATA
		bcf	STATUS,RP1	; Banco 0
		return

;***************************** Subrutina Escribir_EE **********************************
;
; Escribe en la dirección de la EEPROM 'ADDR' el valor contenido en VALUE. Como ADDR y
; VALUE son variables comunes a todos los bancos, la llamada puede ser desde cualquiera  
;
;**************************************************************************************

Escribir_EE	bsf	STATUS,RP1	; Banco 3
		bsf	STATUS,RP0
Espera_WR	btfsc	EECON1,WR	; Espera que termine la escritura anterior
		goto	Espera_WR
		bcf	STATUS,RP0	; Banco 2
		movf	ADDR,0
		movwf	EEADR		; Dirección en la que escribir
		movf	VALUE,0		; Dato a escribir 
		movwf	EEDATA
		bsf	STATUS,RP0	; Banco 3
		bcf	EECON1,EEPGD	; Selecciona Memoria de Datos
		bsf	EECON1,WREN	; Habilita la escritura
		movlw	0x55		
		movwf	EECON2		; EECON2 = 0x55
		movlw	0xAA		; EECON2 = 0xAA
		movwf	EECON2	
		bsf	EECON1,WR	; Empieza la escritura
		bcf	EECON1,WREN	; Inhabilita la escritura
		bcf	STATUS,RP0		
		bcf	STATUS,RP1	; Banco 0
		return

