;***************************************************************************************;
;											;
;					CNFG_TAB.inc					;
;											;
;				Librería con mensajes de CONFIG.inc			;
;											;
;***************************************************************************************;

MenuConfig	ADDWF	PCL,1
		RETLW	'*'
		RETLW	' '
		RETLW	'M'
		RETLW	'e'
		RETLW	'n'
		RETLW	'u'
		RETLW	' '
		RETLW	'C'
		RETLW	'o'
		RETLW	'n'
		RETLW	'f'
		RETLW	'i'
		RETLW	'g'
		RETLW	'.'
		RETLW	' '
		RETLW	'*'
		RETLW	0		;Marcador de fin de mensaje

Linea1		ADDWF	PCL,1
		RETLW	'1'
		RETLW	'.'
		RETLW	' '
		RETLW	'H'
		RETLW	'o'
		RETLW	'r'
		RETLW	'a'
		RETLW	0		;Marcador de fin de mensaje

Linea2		ADDWF	PCL,1
		RETLW	'2'
		RETLW	'.'
		RETLW	' '
		RETLW	'F'
		RETLW	'e'
		RETLW	'c'
		RETLW	'h'
		RETLW	'a'
		RETLW	0		;Marcador de fin de mensaje

Linea3		ADDWF	PCL,1
		RETLW	'3'
		RETLW	'.'
		RETLW	' '
		RETLW	'A'
		RETLW	'f'
		RETLW	'o'
		RETLW	'r'
		RETLW	'o'
		RETLW	0		;Marcador de fin de mensaje

Linea5		ADDWF	PCL,1
		RETLW	'5
		RETLW	'.'
		RETLW	' '
		RETLW	'P'
		RETLW	'I'
		RETLW	'N'
		RETLW	' '
		RETLW	'M'
		RETLW	'a'
		RETLW	'e'
		RETLW	's'
		RETLW	't'
		RETLW	'r'
		RETLW	'o'
		RETLW	0		;Marcador de fin de mensaje

Linea6		ADDWF	PCL,1
		RETLW	'6'
		RETLW	'.'
		RETLW	' '
		RETLW	'A'
		RETLW	'c'
		RETLW	'e'
		RETLW	'r'
		RETLW	'c'
		RETLW	'a'
		RETLW	' '
		RETLW	'd'
		RETLW	'e'
		RETLW	'.'
		RETLW	'.'
		RETLW	'.'
		RETLW	0		;Marcador de fin de mensaje

FormatoHora	ADDWF	PCL,1
		RETLW	' '
		RETLW	' '
		RETLW	' '
		RETLW	' '
		RETLW	' '
		RETLW	'h'
		RETLW	'h'
		RETLW	':'
		RETLW	'm'
		RETLW	'm'
		RETLW	0		;Marcador de fin de mensaje

FormatoFecha	ADDWF	PCL,1
		RETLW	' '
		RETLW	' '
		RETLW	' '
		RETLW	' '
		RETLW	'a'
		RETLW	'a'
		RETLW	'/'
		RETLW	'm'
		RETLW	'm'
		RETLW	'/'
		RETLW	'd'
		RETLW	'd'
		RETLW	0		;Marcador de fin de mensaje

Visitante	ADDWF	PCL,1
		RETLW	'V'
		RETLW	'i'
		RETLW	's'
		RETLW	'i'
		RETLW	't'
		RETLW	'a'
		RETLW	'n'
		RETLW	't'
		RETLW	'e'
		RETLW	':'
		RETLW	' '
		RETLW	0		;Marcador de fin de mensaje

Personas	ADDWF	PCL,1
		RETLW	' '
		RETLW	'p'
		RETLW	'e'
		RETLW	'r'
		RETLW	's'
		RETLW	'o'
		RETLW	'n'
		RETLW	'a'
		RETLW	's'
		RETLW	0		;Marcador de fin de mensaje


