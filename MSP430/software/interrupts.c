#include <msp430x14x.h>
#include <stdbool.h>


//FUNCIÓN: Rutina de Tratamiento de la Interrupción del TIMER B en modo captura del 
// registro CCR0. Se cargó con una cuenta que hace interrumpir cada segundo, así se
// genera un Reloj en Tiempo Real.
// Además en esta Rutina se comprueban los siguientes tiempos: minutos que lleva 
// regando en modo Caudal, desconexión de alguna zona, programa[i].hora_on, 
// programa[i].hora_off , la desconexión del Motor o la llegada a 0 de la 'cuenta'.
// También se activan o desactivan las Unidades de Riego correspondientes, si 
// se han cumplido alguno de los tiempos de hora_on/hora_off.
// Además si el uC no se alimenta con la batería, desde aquí se activará el CAD.
#pragma vector=TIMERB0_VECTOR
__interrupt void CLOCK(void)
{
 unsigned int DiasEnMes, i, j;

 if (date.hora.seg == 59){
    date.hora.seg = 00;
    nuevomin = 1;
    if (date.hora.min == 59){
      date.hora.min = 00;
      if (date.hora.hr == 23){
        date.hora.hr = 00;        
        if (mes_impar&(date.fecha.dd == 31)){
          DiasEnMes = 31;
        }else if (mes_par&(date.fecha.dd == 30)){
          DiasEnMes = 30;
        }else if ((date.fecha.mes==2)&(date.fecha.aa%4 != 0)){
          DiasEnMes = 28;
        }else if ((date.fecha.mes==2)&(date.fecha.aa%4 == 0)){
          DiasEnMes = 29;
        }
        if (date.fecha.dd == DiasEnMes){
          date.fecha.dd = 01;
          if (date.fecha.mes==12){
            date.fecha.mes = 01;
            if (date.fecha.aa == 99){
              date.fecha.aa = 00;
            }else{ date.fecha.aa++; }
            date.fecha.mes++;
          }else{  date.fecha.mes++; }
        }else{  date.fecha.dd++; }
      }else{ date.hora.hr++; }  
    }else{ date.hora.min++; }  
  }else{ date.hora.seg++; }

if(!bat){
///////////   
  if((cuenta < 5) & (cuenta != 0)){
   ON = 12;
   modo = 2;
  } 
  if(cuenta == 0){           //si no actúa la batería, debe volver a pantalla ppal si cuenta == 60.
   //volvemos a medir el caudal si está activada la opción, cada tmpo_pantalla_ppal.
   if((tmpo_caudal) & (vez > 1)){ 
    l = conf[1].io/(frec_user*periodo);  //litros/min correspondientes a la frecuencia medida.
   }  
   cuenta = tmpo_pantalla_ppal; 
   ON = 12;
   modo = 2;
  }else{
    cuenta--;
  }                         
////////// 
  if(nuevomin){
    despierto;
    if(tmpo_caudal){
      if(!bitest(TACCTL2, CCIFG)){    //si no hay señal en CCI2A, pone el flag a '0'.
        vez = 0;
        l = 0;
      }    
      for(i=0; i<veces; i++){
        if((programa[i].hora_off.min!=0)&(esta_regando[i])){     //si ha pasaso 1minuto desde que ocurriera la primera interrup. de CAUDAL.
          programa[i].hora_off.seg++;        //se incrementan los minutos que lleva regando.
        }
      }//for    
    }  
  } 
//comprobamos si se ha cumplido el tiempo de desconexión de alguna UD, empieza o termina algún programa.
    for(i=0; i<6; i++){
      if((zona[i].desconex) & Iguales(zona[i].hora_off, date.hora)){
        out[(zona[i].sal)-1].libre = 1;            //Apagamos UD
        bitclr(RELE_OUT, (zona[i].sal-1)); 
      }
      for(j=0; j<6; j++){
        if(programa[i].ud_on[j]){
          dayofweek = Dia_Semana();
          if((Iguales(programa[i].hora_on, date.hora))&(dias[dayofweek])){
            esta_regando[i] = 1;
            out[(zona[j].sal)-1].libre = 0;          //Encendemos UD por tmpo o caudal.
            bitset(RELE_OUT, (zona[j].sal-1)); 
          }else if((!tmpo_caudal)&((!programa[i].ud_off[j]) & Iguales(programa[i].hora_off, date.hora))){//Si no hay nada que apagar, no entrará en esta condición.
            esta_regando[i] = 0;
            out[(zona[j].sal)-1].libre = 1;          //Apagamos UD
            bitclr(RELE_OUT, (zona[j].sal-1)); 
          }
        }  
      }               
    }
//  if(conf[2].on){            //Si está configurado el MOTOR, comprobamos el Sensor de Nivel.
    ADC12CTL0 |= ADC12SC;    //Convierte cada segundo.
//  }  
  
// Si ha sido configurado Filtrado y su alarma, testea su entrada cada segundo, 
// Si hay una alarma de Filtrado, se irá a la pantalla ppal y se dormirá.
  if(conf[0].on){  
    if(((P4IN&0x80) == 0) & (!filtrando)){ //Si está filtrando, parar de regar.    
      filtrando = 1;
      if(alarma[0].on){
        alarma[0].num_alarmas++;
        if(alarma[0].num_alarmas == 1){ //si es la primera alarma, guarda la hora y fecha
          alarma[0].hora = date.hora;
          alarma[0].fecha = date.fecha;
        }   
      }  
      if((conf[2].on)&(motor)){  //si está configurado y activo el motor
        RELE_OUT &= 0x80;       //Apaga todos los relés menos el correspondiente al motor.
      }else{                    //sino, apaga todos los relés. 
        RELE_OUT = 0;
      }    
    }else if (((P4IN&0x80) != 0) & (filtrando)){  // Ha parado de Filtrar.
      filtrando = 0;
      OnOff_Rele();            //Apaga y enciende las uds que estaban configuradas.  
    }
  }
} 
}

//***********************************************************************************
//FUNCION: Rutina de Tratamiento de la Interrupción del TIMER A en modo captura del
// registro CCI2A para la medida de la frecuencia del tren de pulsos que genera el 
// Sensor de Caudal. En esta interrupción se calcula el período de la señal de entrada
// en el pin 3 del puerto 1 para calcular los minutos que debe regar el sistema, 
// de acuerdo con el caudal y la frecuencia configurada por el usuario.
#pragma vector=TIMERA1_VECTOR
__interrupt void CAUDAL(void)
{
 unsigned int i, j;
 
 if(TAIV == CCI2A){
   periodo = ((float)TACCR2)/((float)32768);  //Tmáx = 2seg-> 0xFFFF máx cuenta de TA.    
   periodo = periodo + 0.0017761231;     //compensamos el error.
   if(vez == 1){                         //inicialmente medimos el caudal
    l = conf[1].io/(frec_user*periodo);  //litros/min correspondientes a la frecuencia medida.   
   }
   
   for(i=0; i<veces; i++){               //comprueba si está regando cada zona.
    if((tmpo_caudal) & (esta_regando[i])){
      if(l!=0){
        litros[i] = programa[i].hora_off.hr - (l*programa[i].hora_off.seg); //actualiza el valor de litros q kedan por vertir.
        programa[i].hora_off.min = litros[i]/l;//
        if(programa[i].hora_off.min == 0){//han pasado los minutos que debe estar regando.
          programa[i].hora_off.seg = 0; //inicialización.
          litros[i] = programa[i].hora_off.hr; //inicialización.
          esta_regando[i] = 0;
          for(j=0; j<6; j++){
              if(programa[i].ud_off[j] == 0){     //Si queremos apagar la Ud 'j', 
                out[(zona[j].sal)-1].libre = 1;   //desactivamos el relé correspondiente.
                bitclr(RELE_OUT, (zona[j].sal-1));
              } //******si sal = 9, no importa pq no existe out[8].<-------------------------------------------   
              else{
                esta_regando[i] = 1;
              }
          }//for
        }
      } 
    }      
   } 
   vez++; 
   TACTL |= TACLR;
 }  
}

//***********************************************************************************
//FUNCION: Rutina de Tratamiento de la Interrupción del Conversor Analógico-Digital
// que interrupe cada segundo si actúa la Alimentación Primaria. Trata de leer desde 
// el registro ADC12MEM0, el valor convertido que le llega del Sensor de Nivel. A 
// partir de éste valor, calculamos el porcentaje de nivel, y comprobamos si se ha 
// acontecido alguna alarma o el nivel ha llegado a algún valor señalado.
// una vez que se haya rebosado o secado el depósito, se activará 1a alarma,
// y no se volverá a resetear a no ser que se borren las alarmas.
#pragma vector=ADC_VECTOR
__interrupt void NIVEL (void)
{
 unsigned long result;
 unsigned int i;
//2978 = 0xBA2 equivale a 2,4V. 
//3723 = 0xE8B que corresponde al valor digital del 3V<-100% 
  result = ADC12MEM0;         // Mueve el resultado, IFG en puesto a '0'.
  if((result < 3723) & (result > 744)){// 0.6V <result< 3V
    result = result - 744;               //ahora el rango será de 0-2.4V
    nivel = (100*result)/2978;           //calcula el nivel en %     
  }else if(result >= 3723){//si result >= 3.3
    nivel = 101;
  }else if(result <= 744){//si result <= 0.6
    nivel = 0;
  }
                               
  switch (nivel){
    case 101: if(alarma[1].on){     //depósito rebosado nivel>100%
                if(alarma[1].num_alarmas == 0){
                  alarma[1].hora = date.hora;
                  alarma[1].fecha  = date.fecha;
                }
                alarma[1].num_alarmas = 1;
                bitclr(RELE_OUT,7); //Apaga el Motor salida 8 puesta a 0.
              }
              break;
    case 0:   if(alarma[2].on){     //depósito seco.
                 if(alarma[2].num_alarmas == 0){                  
                  alarma[2].hora = date.hora;
                  alarma[2].fecha = date.fecha;
                 }
                 alarma[2].num_alarmas = 1; 
                bitset(RELE_OUT,7); //salida 8 puesta a 1.  
              } 
              break;
    default: for(i=0; i<veces; i++){
              if((conf[2].on)&(!esta_regando[i]) & (nivel < porcentaje_llenado)){
                bitset(RELE_OUT,7); //salida 8 puesta a 1.
              }else if ((conf[2].on)&(esta_regando[i]) & (nivel < porcentaje_riego)){
                bitset(RELE_OUT,7); //salida 8 puesta a 1. 
              }else if (conf[2].on){
                bitclr(RELE_OUT,7); //salida 8 puesta a 0. Apaga el Motor. 
              }
             }
             break; 
  };
if(conf[2].on){  //Si está configurado el Motor
  if((nivel != 100) & (Iguales(hora_llenado, date.hora))){//si no está rebosando el depósito
    out[7].libre = 0;   
    bitset(RELE_OUT, 7);  //activar el Motor.
  }
  if(Iguales(motor_off, date.hora)){//aunque nivel = 0, si el usuario indica, apagar motor, yo lo apago.
    out[7].libre = 1;    //Apagamos motor
    bitclr(RELE_OUT, 7); 
  } 
}     
}

//***********************************************************************************
//FUNCION: Rutina de Tratamiento de la Interrupción del COMPARADOR A para equiparar
// el nivel de tensión del pin de entrada 3 del puerto 2(BAT SUP) con una referencia
// interna de 1.65V. Al ser la comparación(CAOUT) = 0, implica la activación de la 
// Batería, y si CAOUT = 1, es porque BAT SUP >0.5*Vcc significa que se está 
// alimentamdo al sistema con la Alimentación principal.
#pragma vector=COMPARATORA_VECTOR
__interrupt void BAT (void)
{
 if(!bitest(CACTL2, BIT0)){    //Si CAOUT es '0', es porque BAT SUP<0.5*Vcc
  bat = 1;
  P1IE  = 0x00;                //Inhabilito interrupción de pulsadores.
  RELE_OUT = 0;                //Se apagarán todas las salidas.
  CCTL2 &= ~CCIE;              //Inhabilito TA
  ADC12CTL0 &= ~ADC12ON;       //Inhabilito CAD
  CACTL1 &= ~CAIES;            //siguiente interrupcion con flanco de subida de CAOUT
  IE1 &= ~URXIE0;              //deshabilita las interrupciones de USART0 RX/TX
  IE1 &= ~UTXIE0;               
  IE1 &= ~WDTIE;               //Inhabilita el timer del WatchDog.
  dormido;                     //Entra en LPM3 
 }else if(bat){                //Si vuelve la Alimentación(12V) se despertará.
  bat = 0;                     //No se usa la Batería.
  P1IE  = 0x70;                //Habilitación de interrupción.
  OnOff_Rele();                //Encendemos y apagamos las salidas ya configuradas.
  CCTL2 |= CCIE;               //Habilito TA
  CACTL1 |= CAIES;             //siguiente interrupcion con flanco de bajada de CAOUT
  IE1 |= URXIE0 + UTXIE0;      // Enable USART0 RX/TX interrupt
  IE1 |= WDTIE;                //Habilita el timer del WatchDog.
  Init_Display();              //inicialización LCD
  despierto;
 }
}

//***********************************************************************************
//FUNCION: Rutina de Tratamiento de Interrupción de uno de los tres pines del Puerto 1
// capacitado para producir una interrupción con flanco de bajada.
// Se encarga de habilitar el TIMER B configurado para descubrir si se ha pulsado en 
// modo 2.
#pragma vector=PORT1_VECTOR
__interrupt void teclado (void)
{
 unsigned int fila1, fila2, fila3;
 unsigned int i;

 P1IE  = 0x00;
 IE1 |= WDTIE;                         // Enable WDT interrupt

 cuenta = tmpo_pantalla_ppal;   

 fila1 = bitest(P1IFG,BIT4);
 fila2 = bitest(P1IFG,BIT5);
 fila3 = bitest(P1IFG,BIT6);
 
 if(fila1 | fila2 | fila3){
  test_boton();
 } 
 P1IFG = 0;   //hay q resetear este registro vía software.   
 if((!manual) & (!bitest(P6OUT,BIT7))){//si led verde on, manual 1.
   manual = 1;
   RELE_OUT = 0; 
   Limpia_Pantalla();
   Put_Str(menumanual);    
   dormido;
 }else if((flanco) & (ON == 2)){    //si apagamos 'manual'.
   ON = 9;
   manual = 0;
   P6OUT = 0xFF;         //Se apagarán los leds de teclado
   //volvemos a configurar el array de registros out[], según la configuración de las zonas
   for(i=0; i<6; i++){
     if(zona[i].on){
      out[zona[i].sal-1].libre = 0;    //salida ocupada
      bitset(RELE_OUT, (zona[i].sal-1));        
     }else{
      out[zona[i].sal-1].libre = 1;    //salida libre
      bitclr(RELE_OUT, (zona[i].sal-1));       
     }  
   }
   OnOff_Rele();
   cuenta = 0;
   despierto;
 }
 if (manual){
      switch(ON){
      case 0 : if(!bitest(P6OUT,BIT5)){//Si el led está encendido,
                out[(zona[0].sal)-1].libre = 0; //Salida ocupada
                bitset(RELE_OUT, (zona[0].sal-1)); 
               }else{
                out[(zona[0].sal)-1].libre = 1; //Salida libre
                bitclr(RELE_OUT, (zona[0].sal-1));  
               } 
               break;
      case 10: if(!bitest(P6OUT,BIT3)){//Si el led está encendido,
                out[(zona[1].sal)-1].libre = 0; //Salida ocupada
                bitset(RELE_OUT, (zona[1].sal-1)); 
               }else{
                out[(zona[1].sal)-1].libre = 1; //Salida libre
                bitclr(RELE_OUT, (zona[1].sal-1)); 
               } 
               break;
      case 20: if(!bitest(P6OUT,BIT1)){//Si el led está encendido,
                out[(zona[2].sal)-1].libre = 0; //Salida ocupada
                bitset(RELE_OUT, (zona[2].sal-1)); 
               }else{
                out[(zona[2].sal)-1].libre = 1; //Salida libre
                bitclr(RELE_OUT, (zona[2].sal-1)); 
               } 
               break;
      case 1 : if(!bitest(P6OUT,BIT4)){//Si el led está encendido,
                out[(zona[3].sal)-1].libre = 0; //Salida ocupada 
                bitset(RELE_OUT, (zona[3].sal-1)); 
               }else{
                out[(zona[3].sal)-1].libre = 1; //Salida libre
                bitclr(RELE_OUT, (zona[3].sal-1)); 
               } 
               break;
      case 11: if(!bitest(P6OUT,BIT2)){//Si el led está encendido,
                out[(zona[4].sal)-1].libre = 0; //Salida ocupada
                bitset(RELE_OUT, (zona[4].sal-1)); 
               }else{
                out[(zona[4].sal)-1].libre = 1; //Salida libre
                bitclr(RELE_OUT, (zona[4].sal-1)); 
               } 
               break;
      case 21: if(!bitest(P6OUT,BIT0)){//Si el led está encendido,
                out[(zona[5].sal)-1].libre = 0; //Salida ocupada
                bitset(RELE_OUT, (zona[5].sal-1)); 
               }else{
                out[(zona[5].sal)-1].libre = 1; //Salida libre
                bitclr(RELE_OUT, (zona[5].sal-1)); 
               } 
               break;
    }
    dormido; 
    ON = 9;
 }     
}

//***********************************************************************************
//FUNCION: Rutina de Tratamiento de Interrupción del WDTimer para averiguar en qué
// modo se ha pulsado la tecla.
// Inetrrumpirá cuando pulsada una tecla, hayan pasada 16mseg, cuenta cargada en el
// registro contador del WDT.
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
 unsigned int num1, num2, num3;
 num1 = bitest(P1IN,BIT4);
 num2 = bitest(P1IN,BIT5);
 num3 = bitest(P1IN,BIT6);
 flanco = num1 & num2 & num3;
         
 if(!flanco){
   tmpo_modo++;                    
   if(tmpo_modo > 70){
    modo = 2;
    flanco = 1;
    tmpo_modo = 0;    
    IE1 &= ~WDTIE; 
    P1IE  = 0x70;
    despierto;
   }
 }else if(tmpo_modo<71){
   modo = 1;
   tmpo_modo = 0;  
   IE1 &= ~WDTIE; 
   P1IE  = 0x70;
   despierto;
 }  
}

