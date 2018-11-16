;***************************************************************************************;
;											;
;					TRJT_TAB.inc					;
;											;
;			Librería con los mensajes de TARJETA.inc			;
;											;
;***************************************************************************************;

extraiga	ADDWF	PCL,1
		RETLW	'E'
		RETLW	'x'
		RETLW	't'
		RETLW	'r'
		RETLW	'a'
		RETLW	'i'
		RETLW	'g'
		RETLW	'a'
		RETLW	' '
		RETLW	0		;Marcador de fin de mensaje

tarjeta		ADDWF	PCL,1
		RETLW	't'
		RETLW	'a'
		RETLW	'r'
		RETLW	'j'
		RETLW	'e'
		RETLW	't'
		RETLW	'a'
		RETLW	0		;Marcador de fin de mensaje


erronea		ADDWF	PCL,1
		RETLW	' '
		RETLW	'e'
		RETLW	'r'
		RETLW	'r'
		RETLW	'o'
		RETLW	'n'
		RETLW	'e'
		RETLW	'a'
		RETLW	'.'
		RETLW	0		;Marcador de fin de mensaje

PIN_error	ADDWF	PCL,1
		RETLW	'P'
		RETLW	'I'
		RETLW	'N'
		RETLW	'('
		RETLW	'e'
		RETLW	'r'
		RETLW	'r'
		RETLW	'o'
		RETLW	'r'
		RETLW	'='
		RETLW	0		;Marcador de fin de mensaje

AforoCompleto	ADDWF	PCL,1
		RETLW	' '
		RETLW	'A'
		RETLW	'f'
		RETLW	'o'
		RETLW	'r'
		RETLW	'o'
		RETLW	' '
		RETLW	'c'
		RETLW	'o'
		RETLW	'm'
		RETLW	'p'
		RETLW	'l'
		RETLW	'e'
		RETLW	't'
		RETLW	'o'
		RETLW	0		;Marcador de fin de mensaje

BaunNoValido	ADDWF	PCL,1
		RETLW	'B'
		RETLW	'.'
		RETLW	' '
		RETLW	'a'
		RETLW	'u'
		RETLW	'n'
		RETLW	' '
		RETLW	'n'
		RETLW	'o'
		RETLW	' '
		RETLW	'v'
		RETLW	'a'
		RETLW	'l'
		RETLW	'i'
		RETLW	'd'
		RETLW	'o'
		RETLW	0		;Marcador de fin de mensaje

Tiene 		ADDWF	PCL,1
		RETLW	'T'
		RETLW	'i'
		RETLW	'e'
		RETLW	'n'
		RETLW	'e'
		RETLW	' '
		RETLW	0		;Marcador de fin de mensaje

Quedan		ADDWF	PCL,1
		RETLW	'Q'
		RETLW	'u'
		RETLW	'e'
		RETLW	'd'
		RETLW	'a'
		RETLW	'n'
		RETLW	' '
		RETLW	0		;Marcador de fin de mensaje

tickets		ADDWF	PCL,1
		RETLW	' '
		RETLW	't'
		RETLW	'i'
		RETLW	'c'
		RETLW	'k'
		RETLW	'e'
		RETLW	't'
		RETLW	's'
		RETLW	0		;Marcador de fin de mensaje

Nuevo_PIN	ADDWF	PCL,1
		RETLW	' '
		RETLW	'N'
		RETLW	'u'
		RETLW	'e'
		RETLW	'v'
		RETLW	'o
		RETLW	' '
		RETLW	'P'
		RETLW	'I'
		RETLW	'N'
		RETLW	':'
		RETLW	' '
		RETLW	0		;Marcador de fin de mensaje

BonoCaducado	ADDWF	PCL,1
		RETLW	' '
		RETLW	'B'
		RETLW	'o'
		RETLW	'n'
		RETLW	'o'
		RETLW	' '
		RETLW	'c'
		RETLW	'a'
		RETLW	'd'
		RETLW	'u'
		RETLW	'c'
		RETLW	'a'
		RETLW	'd'
		RETLW	'o'
		RETLW	0		;Marcador de fin de mensaje

Entrar		ADDWF	PCL,1
		RETLW	'1'
		RETLW	'.'
		RETLW	' '
		RETLW	'E'
		RETLW	'n'
		RETLW	't'
		RETLW	'r'
		RETLW	'a'
		RETLW	'r'
		RETLW	0		;Marcador de fin de mensaje

Recargar	ADDWF	PCL,1
		RETLW	'1'
		RETLW	'.'
		RETLW	' '
		RETLW	'R'
		RETLW	'e'
		RETLW	'c'
		RETLW	'a'
		RETLW	'r'
		RETLW	'g'
		RETLW	'a'
		RETLW	'r'
		RETLW	0		;Marcador de fin de mensaje

Cambiar   	ADDWF	PCL,1
		RETLW	'2'
		RETLW	'.'
		RETLW	' '
		RETLW	'C'
		RETLW	'a'
		RETLW	'm'
		RETLW	'b'
		RETLW	'i'
		RETLW	'a'
		RETLW	'r'
		RETLW	' '
		RETLW	'P'
		RETLW	'I'
		RETLW	'N'
		RETLW	0		;Marcador de fin de mensaje

Ver       	ADDWF	PCL,1
		RETLW	'3'
		RETLW	'.'
		RETLW	' '
		RETLW	'V'
		RETLW	'e'
		RETLW	'r'
		RETLW	' '
		RETLW	'S'
		RETLW	'a'
		RETLW	'l'
		RETLW	'd'
		RETLW	'o'
		RETLW	0		;Marcador de fin de mensaje

Abandonar	ADDWF	PCL,1
		RETLW	'4'
		RETLW	'.'
		RETLW	' '
		RETLW	'A'
		RETLW	'b'
		RETLW	'a'
		RETLW	'n'
		RETLW	'd'
		RETLW	'o'
		RETLW	'n'
		RETLW	'a'
		RETLW	'r'
		RETLW	0		;Marcador de fin de mensaje

InserteDinero	ADDWF	PCL,1
		RETLW	' '
		RETLW	'I'
		RETLW	'n'
		RETLW	's'
		RETLW	'e'
		RETLW	'r'
		RETLW	't'
		RETLW	'e'
		RETLW	' '
		RETLW	'd'
		RETLW	'i'
		RETLW	'n'
		RETLW	'e'
		RETLW	'r'
		RETLW	'o'
		RETLW	0		;Marcador de fin de mensaje

Validez1mes	ADDWF	PCL,1
		RETLW	'('
		RETLW	'v'
		RETLW	'a'
		RETLW	'l'
		RETLW	'i'
		RETLW	'd'
		RETLW	'e'
		RETLW	'z'
		RETLW	' '
		RETLW	'u'
		RETLW	'n'
		RETLW	' '
		RETLW	'm'
		RETLW	'e'
		RETLW	's'
		RETLW	')'
		RETLW	0		;Marcador de fin de mensaje

Recargando 	ADDWF	PCL,1
		RETLW	' '
		RETLW	' '
		RETLW	' '
		RETLW	'R'
		RETLW	'e'
		RETLW	'c'
		RETLW	'a'
		RETLW	'r'
		RETLW	'g'
		RETLW	'a'
		RETLW	'n'
		RETLW	'd'
		RETLW	'o'
		RETLW	0		;Marcador de fin de mensaje


