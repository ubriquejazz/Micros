REM *********************************************
REM ******* Especificaciones del HW FT269 *******
REM *********************************************

RESET$ = "A": REM Respuesta al RESET
REM                     formato: STX A ETX
REM                     respuesta: STX byte0 byte1 byte2 byte3 ETX

RDMM1$ = "B": REM Lectura 1 byte MAIN MEMORY (direcci¢n dir)
REM                     formato: STX B dir ETX
REM                     respuesta: STX byte ETX

RDMMN$ = "C": REM Lectura n bytes MAIN MEMORY (a partir del byte dir)
REM                     formato: STX C dir n ETX
REM                     respuesta: STX byte0 ... byten ETX

RDMMA$ = "D": REM Dump MAIN MEMORY (256 bytes)
REM                     formato: STX D ETX
REM                     respuesta: STX byte0 ... byte255 ETX

WRMM$ = "E": REM Escritura 1 byte MAIN MEMORY (direcci¢n dir)
REM                     formato: STX E dir byte ETX
REM                     respuesta: ACK

RDPMA$ = "F": REM Dump PROTECTION MEMORY (32 bits)
REM                     formato: STX F ETX
REM                     respuesta: STX bit0 ... bit31 ETX

WRPM$ = "G": REM Escritura 1 bit PROTECTION MEMORY
REM                     formato: STX G dir bit ETX
REM                     respuesta: ACK

RDSM$ = "H": REM Dump SECURITY MEMORY (4 bytes)
REM                     formato: STX H ETX
REM                     respuesta: STX byte0 ... byte3 ETX

WRSM$ = "I": REM Escritura 1 byte SECURITY MEMORY (direcci¢n dir)
REM                     formato: STX I dir byte ETX
REM                     respuesta: ACK

CMPSC$ = "J": REM Comparaci¢n PSC
REM                     formato: STX J byte0 byte1 byte2 ETX
REM                     respuesta: ACK

WRPSC$ = "K": REM Escritura del PSC
REM                     formato: STX K byte0 byte1 byte2 ETX
REM                     respuesta: ACK

STATO$ = "S": REM Petici¢n de ESTADO (tarjeta presente)
REM                     formato: STX S ETX
REM                     respuesta: STX 0 ETX (tarjeta ausente)
REM                                STX 1 ETX (tarjeta insertada)

BUSY$ = "U": REM Encendido del LED ROJO
REM                     formato: STX U ETX
REM                     respuesta: ACK

READY$ = "V": REM Encendido del LED VERDE
REM                     formato: STX V ETX
REM                     respuesta: ACK

REM NOTA: bytex, dir y n se representan mediante 2 car cteres ASCII
REM       (30H-3FH) cada car cter representa un nibble

REM *************************************
REM *****  LECTOR / PROGRAMADOR DE  *****
REM *****   LA CHIP-CARD SLE 4442   *****
REM *************************************

1 CLS
2 KEY OFF
3 ON ERROR GOTO 60000
4 COLOR 15: LOCATE 5, 10
5 PRINT "CONTROL DE LA CHIP-CARD SLE 4442";
7 COLOR 12: LOCATE 7, 10
9 PRINT "Juan Manuel Gago Ben¡tez";
11 COLOR 13: LOCATE 9, 10
13 PRINT "Pulse una tecla para continuar";
15 A$ = INKEY$
17 IF A$ = "" GOTO 15
20 DEFINT C, I, R
30 OPEN "com2:9600,E,7,1,CD,DS,CS" FOR RANDOM AS #1
35 GOSUB 17000
40 STX$ = CHR$(2): ETX$ = CHR$(3): ENQ$ = CHR$(5): ACK$ = CHR$(6): NAK$ = CHR$(21)

70 COLOR 14: LOCATE 2, 3, 0: PRINT "SELECCIONE UNA FUNCION:";
71 COLOR 11
72 LOCATE 4, 3, 0: PRINT "A - RESPUESTA AL RESET: Leer MANUFACTURER CODE";

73 LOCATE 6, 3, 0: PRINT "B - LEER 1 BYTE MAIN MEMORY:  Escribir su direcci¢n en HEX (00 - FF)";
74 LOCATE 7, 3, 0: PRINT "C - LEER n BYTES MAIN MEMORY: Escribir START ADDRESS y el NéMERO de bytes";
75 LOCATE 8, 3, 0: PRINT "D - DUMP MAIN MEMORY (256 bytes: direcciones 00 a FF)";
76 LOCATE 9, 3, 0: PRINT "E - ESCRIBIR 1 BYTE EN LA MAIN MEMORY (protegida por el PSC)";
77 LOCATE 10, 3, 0: PRINT "F - DUMP PROTECTION MEMORY (PROM de 32 bit)";

82 LOCATE 12, 3, 0: PRINT "H - DUMP SECURITY MEMORY (32 bits: EC - PSC)";
83 LOCATE 13, 3, 0: PRINT "I - ESCRIBIR 1 BYTE SECURITY MEMORY";
84 LOCATE 14, 3, 0: PRINT "J - COMPARACIàN DEL PSC (formato: byte0 byte1 byte2)";
85 LOCATE 15, 3, 0: PRINT "K - REESCRIBIR   EL PSC (formato: byte0 byte1 byte2)";

92 LOCATE 17, 3, 0: PRINT "U - ENCENDER EL LED ROJO";
93 LOCATE 18, 3, 0: PRINT "V - ENCENDER EL LED VERDE";
94 LOCATE 19, 3, 0: PRINT "S - ¨TARJETA INSERTADA? (1: S¡ / 0: No)";

110 COLOR 13: LOCATE 21, 3, 0: PRINT "0 - FIN";
125 COLOR 15: LOCATE 23, 3, 0: PRINT "FUNCIàN:";
130 LOCATE 23, 24, 0: PRINT " "; : LOCATE 23, 20, 1
135 KB$ = INKEY$
140 IF KB$ = "" GOTO 135
145 IF KB$ > CHR$(&H60) AND KB$ < CHR$(&H7B) THEN KB$ = CHR$(ASC(KB$) - 32)
150 COLOR 12: LOCATE 23, 20: PRINT KB$;
160 IF KB$ = "A" GOTO 1000
161 IF KB$ = "B" GOTO 1500
162 IF KB$ = "C" GOTO 2000
163 IF KB$ = "D" GOTO 2500
166 IF KB$ = "E" GOTO 3000
167 IF KB$ = "F" GOTO 4500
169 IF KB$ = "H" GOTO 5500
170 IF KB$ = "I" GOTO 6000
171 IF KB$ = "J" GOTO 6500
172 IF KB$ = "K" GOTO 7000
178 IF KB$ = "S" GOTO 10000
180 IF KB$ = "U" GOTO 11000
181 IF KB$ = "V" GOTO 11500

200 IF KB$ = "0" THEN END

300 GOSUB 17000
310 GOTO 40

' A - Respuesta al RESET
1000 DT$ = STX$ + RESET$ + ETX$
GOSUB 20000
GOTO 300

' B - Lectura 1 byte MAIN MEMORY
1500 ST$ = "Escriba la direcci¢n del byte (00-FF): ": GOSUB 70000
IF LEN(STT$) = 0 THEN GOTO 300
IF LEN(STT$) > 2 THEN STT$ = LEFT$(STT$, 3)
DT$ = STX$ + RDMM1$ + STT$ + ETX$
GOSUB 20000
GOTO 300

' C - Lectura n bytes MAIN MEMORY
2000 ST$ = "Escriba START ADDRESS (00-FF) y el NéMERO de bytes (00-FF): ": GOSUB 70000
IF LEN(STT$) = 0 THEN GOTO 300
IF LEN(STT$) > 4 THEN STT$ = LEFT$(STT$, 5)
DT$ = STX$ + RDMMN$ + STT$ + ETX$
PRINT DT$;
GOSUB 20000
GOTO 300

' D - Dump MAIN MEMORY
2500 DT$ = STX$ + RDMMA$ + ETX$
GOSUB 20000
GOTO 300

' E - Escritura 1 byte MAIN MEMORY
3000 ST$ = "Escriba START ADRRESS (00-FF) y el BYTE: ": GOSUB 70000
IF LEN(STT$) = 0 THEN GOTO 300
IF LEN(STT$) < 4 THEN GOTO 3000
IF LEN(STT$) > 4 THEN STT$ = LEFT$(STT$, 5)
DT$ = STX$ + WRMM$ + STT$ + ETX$
GOSUB 20000
GOTO 300

' F - Dump PROTECTION MEMORY
4500 DT$ = STX$ + RDPMA$ + ETX$
GOSUB 20000
GOTO 300

' H - Dump SECURITY MEMORY
5500 DT$ = STX$ + RDSM$ + ETX$
GOSUB 20000
GOTO 300

' I - Escritura 1 byte SECURITY MEMORY
6000 ST$ = "Escriba direcci¢n (00-03) y el byte: ": GOSUB 70000
IF LEN(STT$) = 0 THEN GOTO 300
IF LEN(STT$) < 4 THEN GOTO 6000
IF LEN(STT$) > 4 THEN STT$ = LEFT$(STT$, 5)
DT$ = STX$ + WRSM$ + STT$ + ETX$
GOSUB 20000
GOTO 300

' J - Comparaci¢n PSC
6500 ST$ = "Escriba  PSC (000000-FFFFFF): ": GOSUB 70000
IF LEN(STT$) = 0 THEN GOTO 300
IF LEN(STT$) > 6 THEN STT$ = LEFT$(STT$, 7)
DT$ = STX$ + CMPSC$ + STT$ + ETX$
GOSUB 20000
GOTO 300

' K - Escritura PSC
7000 ST$ = "Escriba nuevo PSC (000000-FFFFFF): ": GOSUB 70000
IF LEN(STT$) = 0 THEN GOTO 300
IF LEN(STT$) > 6 THEN STT$ = LEFT$(STT$, 7)
DT$ = STX$ + WRPSC$ + STT$ + ETX$
GOSUB 20000
GOTO 300


' S - Petici¢n de estado
10000 DT$ = STX$ + STATO$ + ETX$
GOSUB 20000
GOTO 300

' U - Encendido LED BUSY
11000 DT$ = STX$ + BUSY$ + ETX$
GOSUB 20000
GOTO 300

' V - Encendido LED READY
11500 DT$ = STX$ + READY$ + ETX$
GOSUB 20000
GOTO 300

17000 CLS : LOCATE 1, 3, 0: COLOR 15
17010 PRINT "CONTROL DE LA CHIP-CARD SLE 4442";
RETURN


18000 COLOR 10: LOCATE 18, 2, 0: PRINT "Comando transmitido:  ";
COLOR 9
PRINT LEFT$(DT$, 2);
FOR I = 3 TO LEN(DT$) - 1
X$ = MID$(DT$, I, 1)
X = ASC(X$)
IF X < 58 THEN PRINT X$;  ELSE PRINT CHR$(X + 7);
NEXT
PRINT RIGHT$(DT$, 1);
RETURN


20000 PRINT #1, DT$;
20050 GOSUB 17000
20100 GOSUB 18000
20300 GOSUB 25000
20400 RETURN



25000 REM An lisis y visualizaci¢n de la respuesta
25020 COLOR 3: LOCATE 25, 2, 0: PRINT "Pulse una tecla para continuar";
25050 COLOR 3: LOCATE 20, 2, 0: PRINT "Respuesta recibida:  ": COLOR 14
25055 ANS$ = ""
25060 FOR I = 1 TO 30000
25070 n = LOC(1)
25080 IF n > 0 THEN ANS$ = ANS$ + INPUT$(n, #1): n = 0: I = 1
25090 NEXT
25100 IF LEN(ANS$) = 0 THEN PRINT " Ninguna respuesta": GOTO 26200
25110 IF LEN(ANS$) = 1 THEN GOTO 25440
25115 FLGCOL% = 0
25120 FOR I = 2 TO LEN(ANS$) - 1
25130 X$ = MID$(ANS$, I, 1)
'25140 IF ASC(X$) < 48 THEN : COLOR 7: X$ = CHR$(ASC(X$) + 16) ELSE COLOR 14
25140 IF (FLGCOL% AND 2) = 0 THEN COLOR 14 ELSE COLOR 11
25300 IF ASC(X$) < 58 AND ASC(X$) > 47 THEN PRINT CHR$(ASC(X$));
25435 IF ASC(X$) > 57 AND ASC(X$) < 64 THEN PRINT CHR$(ASC(X$) + 7);
25436 IF ASC(X$) > 63 THEN PRINT X$;
25437 FLGCOL% = FLGCOL% + 1 AND 255
25438 GOTO 25490
25440 IF ANS$ = CHR$(6) THEN PRINT " ACK";
25450 IF ANS$ = CHR$(21) THEN PRINT " NAK";
25460 IF ANS$ = CHR$(5) THEN PRINT " ENQ";
25480 IF ANS$ = CHR$(7) THEN PRINT " BEL";
25490 IF I = 1024 THEN
25500 A$ = INKEY$: IF A$ = "" THEN GOTO 25500
25510 END IF
25520 NEXT
25600 'PRINT (LEN(ANS$) - 2) / 2;
26200 I$ = INKEY$: IF I$ = "" THEN GOTO 26200
26250 COLOR 7
26300 RETURN

60000 RESUME NEXT

70000 ST$ = ST$ + " ": COLOR 9: LOCATE 25, 1: PRINT " <BACK SPACE>=cancelar   <ESC>=anular"; : COLOR 14
70001 LOCATE 24, 1
70002 FOR I = 1 TO 80
70003 PRINT " ";
70004 NEXT
70005 LOCATE 24, 1: PRINT ST$; : STRN$ = ""
70006 A$ = INKEY$: IF A$ = "" THEN GOTO 70006
70007 IF A$ = CHR$(13) THEN GOTO 70024
70008 IF A$ = CHR$(27) THEN STT$ = "": RETURN
70009 IF A$ = CHR$(8) THEN
70010 A$ = "": X$ = ""
70011 FOR G = 1 TO LEN(STRN$) - 1
70012 X$ = X$ + MID$(STRN$, G, 1)
70013 NEXT
70014 LOCATE 24, LEN(ST$) + 1
70015 FOR G = 1 TO LEN(STRN$)
70016 PRINT " ";
70017 NEXT
70018 STRN$ = X$
70019 END IF
70020 LOCATE 24, LEN(ST$) + 1
70021 IF LEN(STRN$) + LEN(ST$) = 80 THEN BEEP: GOTO 70006
70022 STRN$ = STRN$ + A$: PRINT STRN$; : A$ = ""
70023 GOTO 70006
70024 STT$ = ""
70025 FOR I = 1 TO LEN(STRN$)
70026 X$ = MID$(STRN$, I, 1)
70027 IF X$ > CHR$(96) AND X$ < "g" THEN X$ = CHR$(ASC(X$) - 32)
70030 IF X$ < "0" THEN GOTO 70001
70035 IF X$ > "F" THEN GOTO 70001
70040 IF X$ > "@" AND X$ < "G" THEN X = ASC(X$) - 7: X$ = CHR$(X)
70050 STT$ = STT$ + X$
70060 NEXT
70100 RETURN


END


