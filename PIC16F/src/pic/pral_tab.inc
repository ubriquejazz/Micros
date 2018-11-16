;***************************************************************************************;
;											;
;					PRAL_TAB.inc					;
;											;
;		          Librería con tablas de conversión y mensajes 			;
;											;
;***************************************************************************************;

DiasEnMes	ADDWF	PCL,1
		NOP
		RETLW	1Fh		;Enero
		RETLW	1Ch		;Febrero
		RETLW	1Fh		;Marzo
		RETLW	1Eh		;Abril
		RETLW	1Fh		;Mayo
		RETLW	1Eh		;Junio
		RETLW	1Fh		;Julio
		RETLW	1Fh		;Agosto
		RETLW	1Eh		;Septiembre
		RETLW	1Fh		;Octubre
		RETLW	1Eh		;Noviembre
		RETLW	1Fh		;Diciembre

Conversion	ADDWF	PCL,1
		RETLW	0x00		; Tecla= 0 => 0
		RETLW	0x0D		; Tecla= 1 => D (V )
		RETLW	0x0C		; Tecla= 2 => C	(A )
		RETLW	0x0B		; Tecla= 3 => B	(NO)
		RETLW	0x0A		; Tecla= 4 => A	(OK)		
		RETLW	0x00		; Tecla= 5 => number   	
		RETLW	0x09		; Tecla= 6 => 9
		RETLW	0x06		; Tecla= 7 => 6
		RETLW	0x03		; Tecla= 8 => 3
		RETLW	0x00		; Tecla= 9 => 0
		RETLW	0x08		; Tecla=10 => 8
		RETLW	0x05		; Tecla=11 => 5
		RETLW	0x02		; Tecla=12 => 2
		RETLW	0x00		; Tecla=13 => asterisco	
		RETLW	0x07		; Tecla=14 => 7
		RETLW	0x04		; Tecla=15 => 4
		RETLW	0x01		; Tecla=16 => 1

ConvierteEC	ADDWF	PCL,1
		RETLW	0x03		; EC=0 => 3
		RETLW	0x02		; EC=1 => 2
		RETLW	0x02		; EC=2 => 2
		RETLW	0x01		; EC=3 => 1
		RETLW	0x02		; EC=4 => 2		
		RETLW	0x01		; EC=5 => 1
		RETLW	0x01		; EC=6 => 1
		RETLW	0x00		; EC=7 => 0

MenuUsuario	ADDWF	PCL,1
		RETLW	' '
		RETLW	'M'
		RETLW	'e'
		RETLW	'n'
		RETLW	'u'
		RETLW	' '
		RETLW	'U'
		RETLW	's'
		RETLW	'u'
		RETLW	'a'
		RETLW	'r'
		RETLW	'i'
		RETLW	'o'
		RETLW	' '
		RETLW	0		;Marcador de fin de mensaje

Pulse		ADDWF	PCL,1
		RETLW	'P'
		RETLW	'u'
		RETLW	'l'
		RETLW	's'
		RETLW	'e'
		RETLW	' '
		RETLW	'1'
		RETLW	','
		RETLW	'2'
		RETLW	','
		RETLW	'3'
		RETLW	' '
		RETLW	'o'
		RETLW	' '
		RETLW	'4'
		RETLW	'.'
		RETLW	0		;Marcador de fin de mensaje

