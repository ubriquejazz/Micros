# Modos de direccionamiento (tema 33)

## Direccionamiento inmediato (Literal to W)
En este modo, el operando es un valor constante o literal que se encuentra directamente en la instrucción. Por ejemplo:

    MOVLW 0x0A   ; Cargar el valor literal 0x0A en el registro W

## Direccionamiento directo (W to file)
En este modo, la dirección del operando se especifica directamente en la instrucción. Por ejemplo, esta instrucción se refiere directamente a una ubicación de memoria específica.

    MOVWF 0x20   ; Mover el contenido de W a la variable en la dirección 0x20
 
## Direccionamiento indirecto (file to W)
En este modo, la dirección del operando se encuentra en un registro. Por ejemplo supongamos que el registro INDF se utiliza como puntero para acceder al dato requerido

    MOVF INDF, W   ; Mover el contenido apuntado por INDF a W
    
MOVF puede combinarse con otros modos de direccionamiento y operaciones para realizar diversas tareas y manipulaciones de datos

## INCF (Increment File)
Se utiliza para incrementar (sumar 1) el valor almacenado en un registro y almacenar el resultado nuevamente en el mismo registro. 
La sintaxis de la instrucción es << INCF registro, destino>> << DECF registro, destino>> 

Aquí, "destino" es un indicador de donde se almacenará el resultado (0 - en el registro W; 1 - en el mismo registro)

## ADDWF (Add W to File, store in File) 
Se utiliza para sumar el contenido del registro de trabajo (W) con el contenido de otro registro y almacenar el resultado en el mismo registro. La sintaxis de la instrucción es << ADDWF registro, destino >>

Aquí, "registro" representa el registro de trabajo y "destino" es un indicador de donde se almacenará el resultado (0 - en el registro W; 1 - en el mismo registro)

## SUBLW (Subtract Literal to W)
Se utiliza para restar un valor literal (constante) del registro de trabajo (W) y almacenar el resultado en el registro de trabajo (W).

## BSF (Bit Set File)
Se utiliza para activar (poner a 1) un bit específico en un registro. Su sintaxis es << BSF registro, bit >> Aquí, "registro" representa el nombre del registro en el que se desea establecer el bit, y "bit" es el número del bit (numerado desde 0 hasta 7).

## BCF (Bit Clear File)
La instrucción BCF se utiliza para borrar (poner en 0) un bit específico en un registro. Su sintaxis es similar a la instrucción BSF

    BSF PORTB,0   		; Encender LED conectado a RB0
    BCF STATUS,RP0   	; Regresar al banco de registros 0