            PROCESSOR	16F84      	    ; indicar el procesador
            INCLUDE 	<P16F84.INC> 	; Definiciones del PIC16F84

STATUS	    EQU	0x03
PORTA		EQU	0x05
TRISA		EQU	0x85
PORTB		EQU	0x06
TRISB		EQU	0x86
count		EQU	1			    ; Reserva un espacio para variable
    		ORG 0x0000        	; Dirección de inicio del programa

; Configuración del registro de direcciones de puertos A y B
    		BSF STATUS, RP0		; Entra al banco de registros 1
    		MOVLW b'00000000' 	; Valor de configuración en W
		    MOVWF TRISA		    ; Carga W en el puerto A
    		MOVLW b'00000000' 	; Valor de configuración en W
		    MOVWF TRISB		    ; Carga W en el puerto B
    		BCF STATUS, RP0   	; Regresar al banco de registros 0

 ; Ciclo principal del programa
 loop:
        	BSF PORTB, 0   	; Encender LED conectado a RB0
        	CALL delay     	; Llamar a la subrutina de retardo
        	BCF PORTB, 0   	; Apagar LED conectado a RB0
        	CALL delay     	; Llamar a la subrutina de retardo
        	GOTO loop      	; Volver al inicio del ciclo principal            

; Subrutina de retardo
delay:
        	MOVLW D'250'   	; Cargar valor de retardo
        	MOVWF count    	; Guardar en el registro COUNT
        	DECFSZ count, 0 ; Decrementar COUNT y saltar si 0
        	GOTO $-1       	; Volver a DECFSZ si no es 0
        	RETURN         	; Retornar de la subrutina

END			; Fin del programa            