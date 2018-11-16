#include <string.h>
#include <stdbool.h>
#include <stdio.h>


//ENTRADAS: -
//SALIDAS:  -
//FUNCIÓN: Se encarga de inicializar todas las variables globales.
void Configuracion(void)
{
  unsigned int i;
  
  Init_Ports();      // configura los puertos
  
  cuenta = tmpo_pantalla_ppal;  //Se inicializa la cuenta.
/////inicialización del registro fecha/////
  date.hora.seg  = 00;      
  date.hora.min  = 26;
  date.hora.hr   = 10;
  date.fecha.dd  = 15;
  date.fecha.mes = 5;
  date.fecha.aa  = 05; 
//inicialización del array de registros programa//   
  Limpia_Programa();  
//inicialización del array de registros conf//  
  for(i=0; i<3; i++){
    conf[i].on = 0;      
    conf[i].io = 9;     //por defecto le asignamos salida o entrada libre.
  }
//inicialización del array de registros alarma//
  for(i=0; i<5; i++){ 
    alarma[i].on          = 0;
    alarma[i].num_alarmas = 0;
    alarma[i].hora = Limpia_Hora(alarma[i].hora);
    alarma[i].fecha = Limpia_Fecha(alarma[i].fecha);
  }  
//inicialización del array de registros zona//   
  for(i=0; i<6; i++){
    zona[i].on        = 0;
    zona[i].desconex  = 0;
    zona[i].hora_off = Limpia_Hora(zona[i].hora_off);
    zona[i].remoto    = 0;      //Modo LOCAL.
    zona[i].direccion = 1;      //primera dirección asignable a un esclavo(éste stma tb puede se esclavo)
    zona[i].sal       = i+1;    //por defecto se la asigna a la zona 1 la salida 1, ...
  }
//inicialización del array de registros entradas y salidas//
 for(i = 0; i<8; i++){
  out[i].libre     = 1;
  in[i].libre      = 1;
  out[i].remoto    = 0;      //Por defecto modo LOCAL
  in[i].remoto     = 0;
  out[i].direccion = 1;
  in[i].direccion  = 1;
 } 
 hora_llenado = Limpia_Hora(hora_llenado); 
}

//***********************************************************************************
//ENTRADAS: struct hora h
//SALIDAS: struct hora h
//FUNCIÓN: Inicializa el registro hora.
struct hora Limpia_Hora(struct hora h)
{
 h.seg = 0;
 h.min = 0;
 h.hr  = 0; 
 
 return(h);
}

//***********************************************************************************
//ENTRADAS: struct fecha f
//SALIDAS: struct fecha f
//FUNCIÓN: Inicializa el registro fecha.
struct fecha Limpia_Fecha(struct fecha f)
{
 f.dd  = 0;
 f.mes = 0;
 f.aa  = 0;
 
 return(f);
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Inicializa el array de registros de tipo programa.
void Limpia_Programa(void)
{
 unsigned int i, j;
 
 veces = 1;               
 for(i=0; i<7; i++){
  dias[i] = 0;
 }
 
 for(i=0; i<6; i++){
  for(j=0; j<6; j++){
   programa[i].ud_on[j] = 0;
   programa[i].ud_off[j] = 0;
  }
  programa[i].hora_on = Limpia_Hora(programa[i].hora_on);
  programa[i].hora_off = Limpia_Hora(programa[i].hora_off);
 }//el campo hr se guardarán los LITROS OFF, en min los minutos qdebe contar,
  //y en seg los que lleva contados, cuando tmpo_caudal = 1.
}

//*********************************************************************************//
//***********************     SUBMENU      ****************************************//
//*********************************************************************************//
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Se encarga de Pintar y Ejecutar cada linea del menú, seleccinada por el
// usuario.
void Menu(void)
{
 unsigned int linea = 1;

 Limpia_Pantalla();
 Put_Ins(xy15);           
 Put_Str(menu);
 Put_Ins(LINEA2);
 Linea(linea);
 do{
   if(ON != 9){
    while (flanco == 0){}
    switch (ON){
     case 12: break;              // ESC   
     case 3: ON = 9;              // +
               if (linea == 1){   
                linea = 9;
               }else{ linea--;}
               Linea(linea);
               break;
     case 13: ON = 9;             // -
               if (linea == 9){        
                linea = 1;
               }else{ linea++;}
               Linea(linea);
               break;
     case 22: ON = 9;             //<-|
               Ejecuta(linea);
               Limpia_Pantalla();
               Put_Ins(xy15);
               Put_Str(menu);
               Put_Ins(LINEA2);
               Linea(linea);
               break;
     }
   }
 }while(ON!=12);
 while(flanco == 0){}
 ON = 9; 
 modo = 0;
}

//***********************************************************************************
//ENTRADAS: linea
//SALIDAS: -
//FUNCIÓN: Pinta la línea que se corresponde con el número que le llega de entrada.
void Linea(unsigned int linea)
{
 Limpia_Linea(LINEA2);
 switch(linea){
  case 1: Put_Str(cmanual); break;
    case 11: Put_Str(zonas); break;
    case 12: Put_Str(manualmotor); break;
    case 13: Put_Str(manualestado); break;
  case 2: Put_Str(riego); break;
    case 21: Put_Str(hon); break;
    case 22: if(tmpo_caudal == 0){
              Put_Str(hoff); 
             }else{
              Put_Str(litro); 
             }
             break;
  case 3: Put_Str(estado); break;
  case 4: Put_Str(config); break;
  case 5: Put_Str(conf_IO); break;
    case 51: Put_Str(zonas); break;
    case 52: Put_Str(salidas); break;
    case 53: Put_Str(entradas); break;
    case 54: Put_Str(ver); break;
      case 541: Put_Str(zonas); break;
      case 542: Put_Str(salidas); break;
      case 543: Put_Str(entradas); break;
  case 6: Put_Str(conf_AL); break;
  case 7: Put_Str(reloj); break;
    case 71: Put_Str(hora); break;
    case 72: Put_Str(fech); break;    
  case 8: Put_Str(deposito); break;
    case 81: Put_Str(llenado); break;
    case 82: Put_Str(min_llenado); break;
    case 83: Put_Str(en_riego); break;
  case 9: Put_Str(guarda); break;  
 }
}

//***********************************************************************************
//ENTRADAS: linea
//SALIDAS: -
//FUNCIÓN: Ejecuta la línea que se corresponde con el número que le llega de entrada.
void Ejecuta(unsigned int linea)
{
 Limpia_Pantalla();
 switch(linea){
  case 1: Manual(); break;
    case 11: Unidad(); break;
    case 12: Motor(); break;
    case 13: Estado_Dep(); break;
  case 2: Programacion(); break;
  case 3: Estado(); break;
  case 4: Conf(); break;
  case 5: Conf_IO(); break;
    case 51: Zonas(); break;
    case 52: IN_OUT(1, out, salidas_ocup); break;
    case 53: IN_OUT(0, in, entradas_ocup); break;
    case 54: Ver_Conf(); break;
  case 6: Conf_AL(); break;
  case 7: Reloj(); break;
    case 71: Poner_Hora(5, 0, 0); break;
    case 72: Poner_Fecha(); break;
  case 8: Deposito(); break;
    case 81: Poner_Hora(1, 0, 0); break;
    case 82: Porcentaje(0); break;
    case 83: Porcentaje(1); break;
  case 9: Guarda_Conf(); break;
 }
 while(flanco == 0){}       //se controla mejor la pulsación cuando queremos regresar.
 ON = 9;
 Put_Ins(Set_Display +Display_On + Cursor_Off + Blink_Off);
}

//*********************************************************************************//
//***********************   1.Control Manual    ***********************************//
//*********************************************************************************//
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Primera opción del Menú. Expande el Submenú Control Manual.
void Manual(void)
{
 unsigned int sublinea = 11;

 Put_Str(menumanual);
 Linea(sublinea);
 do{
   if (ON != 9){
    while (flanco == 0){}
     switch (ON){
      case 12: break;                  // ESC
      case 3: ON = 9;                  // -
                if (sublinea == 11){            
                  sublinea = 13;
                }else{ sublinea--;}
                Linea(sublinea);
                break;
      case 13: ON = 9;                 // +
                if (sublinea == 13){   
                  sublinea = 11;
                }else{ sublinea++;}
                Linea(sublinea);
                break;
      case 22: ON = 9;                 //<-|
                Ejecuta(sublinea);
                Limpia_Pantalla();
                Put_Str(menumanual);
                Linea(sublinea);
                break;
     } 
   }
 }while(ON!=12);
 while (flanco == 0){}
 ON = 9;
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Activa/Desactiva las Unidades de Riego seleccionadas. Si pulsamos <-| en 
// modo 2 se activará la desconexión de la ud seleccionada(Poner_Hora). Y volveremos 
// a llamar a Unidad() de forma recursiva.
void Unidad(void)
{
 unsigned int ud;
 
 ud = activa_ud(0, 11);
 if (ON == 22){              //Si desde activación de Uds se kiere activar la desconexión.
  ON = 9;
  modo = 0;
  zona[ud].desconex = 1;
  Limpia_Pantalla();
  Put_Ins(xy16);
  Put_Str(desconex);
  Put_Num(ud+1);             //Escribe la ud(var global) que queremos desactivar.
  Poner_Hora(0, 0, ud);      //aquí se guarda en el campo hora_off, la hora de desconexión.
  Unidad();
 } 
 ON = 9;
 modo = 0;
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Configuramos la activación o no del Motor, y si pulsamos <-| en modo 2
// podremos desconexionar la salida del Motor en el instante indicado(Poner_Hora).
void Motor(void)
{
 Pinta_Conf(3); 
 do{
  if (ON != 9){
   while (flanco == 0){}
    switch (ON){
      case 12: if(modo == 1){
                on_off(0, 2, 0, 12);      //conf_al_ud = 2 y sublínea = 12. 
                Pinta_Conf(3);
               }
               break;              
      case 3 : break;
      case 13: break;
      case 22: if (conf[2].on){
                Limpia_Pantalla();
                Put_Ins(xy16);
                Put_Str(OFF);
                ON = 9;
                modo = 0;
                Poner_Hora(4, 0, 0);
                Put_Ins(Set_Display + Display_On + Cursor_Off + Blink_Off); 
                ON = 9;
                modo = 0;
              }else{
                on_off(0, 2, 0, 12);      //conf_al_ud = 2 y sublínea = 12. 
              }
              Pinta_Conf(3);
              break;    
    }
  }
 }while((ON != 12) | (modo != 2));     
 ON = 9;
 modo = 0;  
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: En ésta función mostramos el nivel actual del depósito, que es captado por
// el CAD desde el sensor de Nivel.
void Estado_Dep(void)
{
 Limpia_Pantalla();
 Put_Ins(xy16);
 Put_Num(nivel);
 Put_Str("%");
 Delayx100us(20000);  //espera 2seg.
}

//***********************************************************************************
//***********************   2.Programa Riego    *************************************
//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Segunda opción del Menú. La forma de riego siempre será configurable por 
// Tiempos, pero sólo la podremos controlar por Caudal, cuando en la opción
// 4.Configuración, activemos Caudal.
void Programacion(void)
{
 Put_Str(riegox);
 Put_Ins(LINEA2); 
 Put_Str(tiempo);
 if(conf[1].on == 1){
  Put_Ins(xy2A);
  Put_Str(caudal);
 }
 if(!tmpo_caudal){
  Put_Ins(xy26);
  Put_Str("<");
 }else{
  Put_Ins(xy29);
  Put_Str(">");    
 } 
 
 do{
   if (ON != 9){
    while (flanco == 0){}
      if ((ON == 12) & (modo == 1)){
        ON = 9;
        tmpo_caudal = 0;
        Dias();                  
      }else if((ON == 22) & (conf[1].on == 1)){  
        ON = 9;
        tmpo_caudal = 1;
        Dias();
    }   
   } 
 }while((ON != 12) | (modo!=2));
 while(flanco == 0){}
 ON = 9;
 modo = 0;
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Esta función, simplemente captura los dias de la semana seleccionados por
// el usuario. Si pulsamos <-| en modo 2 pasamos a la función Veces();
void Dias(void)
{
 cursor = xy21;
 Limpia_Pantalla(); 
 Put_Str(diariego);
 Put_Ins(LINEA2);
 marca(); 
 Put_Ins(cursor);
 Put_Ins(Set_Display + Display_On + Cursor_On + Blink_On);

 do{
   if (ON != 9){
    while (flanco == 0){}
     switch (ON){
       case 12: break;
       case 3: ON = 9;
                 if (cursor == xy21){
                  cursor = xy2D;
                 }else{
                  cursor = cursor - 2;
                 }                 
                 Put_Ins(cursor);                               
                 break;
       case 13: ON = 9;
                 if (cursor == xy2D){
                  cursor = xy21;
                 }else{
                  cursor= cursor + 2;
                 }                 
                 Put_Ins(cursor);                 
                 break;
       case 22: ON = 9;
                 if(modo == 1){
                  switch(cursor){
                    case xy21:if(dias[0]==0){
                                dias[0]=1;
                              }else{ dias[0]=0;}
                              break;  
                    case xy23:if(dias[1]==0){
                                dias[1]=1;
                              }else{ dias[1]=0;}
                              break;  
                    case xy25:if(dias[2]==0){
                                dias[2]=1;
                              }else{ dias[2]=0;}
                              break;  
                    case xy27:if(dias[3]==0){
                                dias[3]=1;
                              }else{ dias[3]=0;}
                              break;  
                    case xy29:if(dias[4]==0){
                                dias[4]=1;
                              }else{ dias[4]=0;}
                              break;  
                    case xy2B:if(dias[5]==0){
                                dias[5]=1;
                              }else{ dias[5]=0;}
                              break;  
                    case xy2D:if(dias[6]==0){
                                dias[6]=1;
                              }else{ dias[6]=0;}
                              break;  
                  }  
                  marca();
                  ON = 13;     //para que el cursor se coloque en una posición más.
                 }else if(modo == 2){
                   Veces();
                   break;
                 }           
       }  
   } 
 }while(ON != 12);
 while(flanco == 0){}
 Put_Ins(Set_Display + Display_On + Cursor_Off + Blink_Off);
 if(modo == 1){
  ON = 9;
  Limpia_Pantalla(); 
  Put_Str(riegox);
  Put_Ins(LINEA2); 
  Put_Str(tiempo);  
  if(conf[1].on == 1){
    Put_Ins(xy2A);
    Put_Str(caudal);
  }
  if(!tmpo_caudal){
    Put_Ins(xy26);
    Put_Str("<");
  }else{
    Put_Ins(xy29);
    Put_Str(">");    
  }
 } 
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Según se hayan seleccionado o no cualquier dia de la semana, su carácter
// se verá invertido o no.
void marca(void)
{
  Put_Ins(xy21);
  if(dias[0]==1){
    Put_Data(0);
  }else{ Put_Str("L");}
  
  Put_Ins(xy23);
  if(dias[1]==1){
    Put_Data(1);
  }else{ Put_Str("M");}

  Put_Ins(xy25);
  if(dias[2]==1){
    Put_Data(2);
  }else{ Put_Str("X");}
  
  Put_Ins(xy27);
  if(dias[3]==1){
    Put_Data(3);
  }else{ Put_Str("J");}
  
  Put_Ins(xy29);
  if(dias[4]==1){
    Put_Data(4);
  }else{ Put_Str("V");}
      
  Put_Ins(xy2B);    
  if(dias[5]==1){
    Put_Data(5);
  }else{ Put_Str("S");}
  
  Put_Ins(xy2D);
  if(dias[6]==1){
    Put_Data(6);
  }else{ Put_Str("D");}         

 Put_Ins(xy21);  
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: El usuario se encargará de insertar el número de veces que quiere regar 
//cada día. Éste número se verá reflejado en la variable 'veces'.
void Veces(void)
{
 Limpia_Pantalla();
 Put_Str(bces);
 
 Put_Ins(LINEA2);
 Put_Str(seis);
 Put_Ins(xy2E);
 Put_Num(veces); 
 Put_Ins(xy2E);
 
 do{
   if (ON != 9){
    while (flanco == 0){}
     switch (ON){
      case 12: break;        //ESC
      case 3 : ON = 9;       // -
               if (veces == 1){               
                 veces = 6;
               }else{ veces--;}
               Put_Num(veces); 
               Put_Ins(xy2E);                            
               break;
      case 13: ON = 9;       // +
               if (veces == 6){             
                 veces = 1;
               }else{ veces++;}
               Put_Num(veces); 
               Put_Ins(xy2E);                                 
               break;
      case 22: ON = 9;       //<-|
               Put_Ins(Set_Display + Display_On + Cursor_Off + Blink_Off);
               programas();
               break;
     }  
   } 
 }while(ON != 12);
 while(flanco == 0){}
 if(modo == 1){
  ON = 9;
  Limpia_Pantalla(); 
  Put_Str(diariego);
  Put_Ins(LINEA2);
  marca(); 
  Put_Ins(xy21);
  Put_Ins(Set_Display + Display_On + Cursor_On + Blink_On);
 } 
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Una vez introducidas las 'veces', pasamos a configurar cada programa, que
// según hayamos seleccionado Tiempo/Caudal, tendremos HORA OFF/LITROS OFF.
void programas(void)
{
 unsigned int i = 1, texto = 21, ud = 0;

  Limpia_Pantalla();
  Put_Str(prog);
  Put_Ins(xy1A);
  Put_Num(i); 
  Put_Ins(LINEA2);
  Linea(texto);
  
  do{
    if (ON != 9){
      while (flanco == 0){}
      switch (ON){
        case 12: break;      //ESC
        case 3: ON = 9;      // -
                  if (modo == 1){
                    if (texto == 21){ 
                      texto = 22;
                    }else{ texto--;}
                    Linea(texto);                   
                  }else{
                     if (i == 1){
                      i = veces;
                     }else{ i--;}
                     Limpia_Pantalla();
                     Put_Str(prog);
                     Put_Ins(xy1A);
                     Put_Num(i); 
                     Put_Ins(LINEA2);
                     Linea(texto); 
                  }   
                  break;
        case 13: ON = 9;     // +
                  if (modo == 1){
                    if (texto == 22){   
                      texto = 21;
                    }else{ texto++;}
                    Linea(texto);                   
                  }else{
                     if (i == veces){
                      i = 1;
                     }else{ i++;}
                     Limpia_Pantalla();
                     Put_Str(prog);
                     Put_Ins(xy1A);
                     Put_Num(i); 
                     Put_Ins(LINEA2);
                     Linea(texto); 
                  }   
                  break;
        case 22: ON = 9;     // <-|
                  modo = 0;
                  if(texto == 21){          //HORA ON
                    Limpia_Pantalla();
                    Put_Ins(xy16);
                    Put_Str(OK);
                    Poner_Hora(2, i, ud);
                    if (ON == 22){
                      ud = activa_ud(i, texto);
                    }
                    ON = 9;
                    modo = 0;
                       
                  }else if ((texto == 22) & (tmpo_caudal == 0)){  //HORA OFF
                    Limpia_Pantalla();
                    Put_Ins(xy16);
                    Put_Str(OFF);                  
                    Poner_Hora(3, i, ud);
                    if (ON == 22){
                      ud = activa_ud(i, texto);
                    }
                    ON = 9;
                    modo = 0;
                     
                  }else if (texto == 22){   // riego por caudal. LITROS OFF
                    Litros(i-1);
                    if (ON == 22){
                      ud = activa_ud(i, texto);
                    }
                    ON = 9;
                    modo = 0;
                  }
                  Limpia_Pantalla();
                  Put_Str(prog);
                  Put_Ins(xy1A);
                  Put_Num(i); 
                  Put_Ins(LINEA2);
                  Linea(texto); 
                  Put_Ins(Set_Display + Display_On + Cursor_Off + Blink_Off);                           
                  break;
      }  
    } 
  }while(ON != 12);
  while(flanco == 0){} 
 
 if(modo == 1){   //da sólo un paso atrás. Si modo == 2, vuelve a Submenú.
  ON = 9;
  Limpia_Pantalla();
  Put_Str(bces);
  Put_Ins(LINEA2);
  Put_Str(seis);
  Put_Ins(xy2E);
  Put_Num(veces);
  Put_Ins(xy2E);
  Put_Ins(Set_Display + Display_On + Cursor_On + Blink_On);
 }
}

//***********************************************************************************
//ENTRADAS: i(número del programa), texto(1.HORA ON/2.HORA OFF/2.LITROS OFF).
//SALIDAS: ud(última Unidad de Riego configurada).
//FUNCIÓN: Si una vez configurada HORA ON/OFF/LITROS OFF pulsamos <-| en modo 2,
// aparecerán las Unidades de Riego para ser Encendidas/Apagadas.
unsigned int activa_ud(unsigned int i, unsigned int texto)
{
 unsigned int ud = 0, conf_al_ud = 2;
 
 ON = 9;
 modo = 0;
 Put_Ins(Set_Display + Display_On + Cursor_Off + Blink_Off);
 ud_onoff(ud, i, texto);
  
  do{
   if (ON != 9){
    while (flanco == 0){}
     switch (ON){
      case 12: if(modo == 1){//OFF
                 on_off(ud, conf_al_ud, i, texto);
                 ud_onoff(ud, i, texto);
                }                             
                break;       //ESC          
      case 3: ON = 9;        // -
                modo = 0;
                if (ud == 0){               
                  ud = 5;
                }else{ ud--;}
                ud_onoff(ud, i, texto);                
                break;
      case 13: ON = 9;       // +
                modo = 0;
                if (ud == 5){               
                  ud = 0;
                }else{ ud++;}
                ud_onoff(ud, i, texto);              
                break;
      case 22: if(modo == 1){//ON
                  on_off(ud, conf_al_ud, i, texto);
                  ud_onoff(ud, i, texto);
                } else if (!uni[ud]){
                  modo = 0;
                }    
                break;
     }  
   } 
  }while(modo!=2);               
  while(flanco == 0){}
  return(ud);  
}

//***********************************************************************************
//ENTRADAS: ud(Unidad de Riego que activaré/desactivaré), i(número del programa),
//          texto(1.HORA ON/2.HORA OFF/2.LITROS OFF).
//SALIDAS: -
//FUNCIÓN: Activamos o desactivamos las Unidades de Riego con su correspondiente 
// Salida a Relé. 
void ud_onoff(unsigned int ud, unsigned int i, unsigned int texto)
{
 unsigned int encendido = 0;
 
 Limpia_Pantalla();
 Pinta_Zona(ud);
 Limpia_Linea(LINEA2);
 Put_Str(OFF);
 Put_Ins(xy2E);
 Put_Str(OK);
 
 if(texto == 21){                      //ud_on
  if (programa[i-1].ud_on[ud]){              
    encendido = 1;
    programa[i-1].ud_off[ud] = 1;
  }
 }else if (texto == 22){               //ud_off
   if (!programa[i-1].ud_off[ud]){             
    encendido = 0;
   }else{
    encendido = 1;
   }
 }else if (texto == 11){          
   if (zona[ud].on){              
    out[(zona[ud].sal)-1].libre = 0;   //Para encender el Relé 
    bitset(RELE_OUT, (zona[ud].sal-1)); 
    encendido = 1;
   } else if (!zona[ud].on){           
    out[(zona[ud].sal)-1].libre = 1;   //Para apagar el Relé
    bitclr(RELE_OUT, (zona[ud].sal-1));      
    encendido = 0;
   }   
 } 

 if (encendido){              //si zona activada ,, señalizarla ">"
   Put_Ins(xy2D);
   Put_Str(">");
 }else if (!encendido){
   Put_Ins(xy23);
   Put_Str("<");
 } 
}

//***********************************************************************************
//ENTRADAS: i(número del programa).
//SALIDAS: -
//FUNCIÓN: El usuario introduce el número de litros que debe medir el sensor de Caudal
// para parar de regar. LITROS OFF se guardarán en programa[i].hora_off.hr, en 
// programa[i].hora_off.min se guardan los minutos que debe regar según el caudal medido
// y en programa[i].hora_off.seg los minutos que lleva regando.
void Litros(unsigned int i)
{
 Limpia_Pantalla();
 Put_Str(litro);
 Put_Ins(LINEA1);
 Put_Str("  ");
 
 Put_Ins(xy23);
 Put_Num(10000);       //pinta los litros
 Put_Num(0); 
 Put_Num(0);
 
 Put_Ins(xy23);
 Put_Ins(Set_Display + Display_On + Cursor_On + Blink_On);
 
 programa[i].hora_off.hr = Numero(lmax, 7, xy23);   //en el campo 'hr' del registro hora_off, guardamos LITROS OFF
 litros[i] = programa[i].hora_off.hr;
 //por esto la variable 'hr' del registro hora será unsigned long, pq puede valer 1000000.
}

//***********************************************************************************
//***********************   3.Estado Alarmas    *************************************
//***********************************************************************************
//ENTRADAS: i(número del programa).
//SALIDAS: -
//FUNCIÓN: Tercera opción del Menú. Informa del estado actual de las 4 alarmas posibles
// en nuestro sistema: Filtrado, Depósito Seco, Despósito rebosado y Error de 
// comunicación.
void Estado(void)
{
 unsigned int texto = 0;

 pinta_al(texto);
 do{
   if (ON != 9){
    while (flanco == 0){}
     switch (ON){
      case 12: break;         //ESC                  
      case 3 : ON = 9;
               if(texto == 0){// -
                 texto = 4;
               }else{ texto--;}
               pinta_al(texto);                             
               break;
      case 13: ON = 9;        // +
                if (texto == 4){ 
                  texto = 0;
                }else{ texto++;}
                pinta_al(texto);                
                break;
      case 22: ON = 9;        // <-|
               if(alarma[texto].num_alarmas!=0){                  
                Ver_al(texto);
                pinta_al(texto);                  
               }  
               break;
     }  
   } 
 }while(ON != 12);
 while(flanco == 0){}
 ON = 9;
 modo = 0;
}

//***********************************************************************************
//ENTRADAS: texto(indica la alarma que se va ha pintar).
//SALIDAS: -
//FUNCIÓN: Según la entrada, pintará una de las 4 alarmas, y señalizará la que se 
// haya producido alguna alarma.
void pinta_al(unsigned int texto)
{
 Limpia_Pantalla();
 Put_Ins(xy14);
 switch(texto){
  case 0: Put_Str(filtro);           
           break;
  case 1: Put_Str(dep);
          Put_Ins(xy24);
          Put_Str(rebosado);
          break;
  case 2: Put_Str(dep);
          Put_Ins(xy26);
          Put_Str(seco);
          break;
  case 3: Put_Str(error);
          Put_Ins(xy24);
          Put_Str(comunic);
          break;
  case 4: Put_Ins(LINEA1);
          Put_Str(borrar);
          Borra();          
          break;           
 }

 if (alarma[texto].num_alarmas != 0){              //si hay alarmas ,, señalizarla "->"
   Put_Ins(xy2E);
   Put_Str(flecha);
 } 
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Se encarga de borrar el array de registros de tipo alarma, menos 
// alarma[i].on, que guarda la configuración de alarmas.
void Borra(void)
{
 unsigned int i;
 
 Put_Ins(LINEA2);
 Put_Str(NO);
 Put_Ins(xy2E);
 Put_Str(SI);
 do{
   if (ON != 9){
    while (flanco == 0){} 
      if(ON == 22){
        Limpia_Pantalla();
        Put_Str(borrada);
        for(i=0; i<5; i++){ 
          alarma[i].num_alarmas = 0;
          alarma[i].hora = Limpia_Hora(alarma[i].hora);
          alarma[i].fecha = Limpia_Fecha(alarma[i].fecha);
        }
        Delayx100us(20000);        //espera 2seg. 
        Put_Ins(LINEA1);
        Put_Str(borrar); 
        Put_Ins(LINEA2);
        Put_Str(NO);
        Put_Ins(xy2E);
        Put_Str(SI);      
      }
   } 
 }while((ON!=12) & (ON!=22) & (ON!=3) & (ON!=13));
 while(flanco == 0){}
 modo = 0; 
}

//***********************************************************************************
//ENTRADAS: texto(indica la alarma que se va ha pintar).
//SALIDAS: -
//FUNCIÓN: Muestra, de la alarma que se ha producido, el número de ellas y la hora y
// fecha de la primera alarma.
void Ver_al(unsigned int texto)
{
 Put_Ins(xy12);
 Put_Num(alarma[texto].num_alarmas);
 Put_Str(alarmas);
 Put_Ins(LINEA2);
 Put_Num(1); Put_Str("a ");
 Hora(alarma[texto].hora.min, alarma[texto].hora.hr);
 Fecha(alarma[texto].fecha.dd, alarma[texto].fecha.mes, alarma[texto].fecha.aa);
 do{
   if (ON != 9){
    while (flanco == 0){} 
   } 
 }while((ON!=12) & (ON!=22));
 while(flanco == 0){}
 ON = 9;
 modo = 0; 
}

//***********************************************************************************
//***********************   4.Configuración    **************************************
//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Cuarta opción del Menú. Se encarga de activar/desactivar el FILTRO, CAUDAL 
// y MOTOR. Al salir de ésta opción, si Filtrado y Motor se han activado, se les 
// asignarán la entrada 8 y la salida 8, respectivamente.
void Conf(void)
{
 unsigned int texto = 0;
 unsigned int conf_al = 0;

 Pinta_Conf(texto);
 do{
   if (ON != 9){
    while (flanco == 0){}
     switch (ON){
     case 12: if(modo == 1){
                 on_off(texto, conf_al,0, 0);
                 Pinta_Conf(texto);
                }else if(modo == 2){}           //Si boton 12 pulsado en modo 2,,vuelvo a menu
                break;                          // NO
      case 3: ON = 9;
                if (texto == 0){               //arriba
                  texto = 2;
                }else{ texto--;}
                Pinta_Conf(texto);                             
                break;
      case 13: ON = 9;
                if (texto == 2){               // abajo
                  texto = 0;
                }else{ texto++;}
                Pinta_Conf(texto);                
                break;
      case 22: if((texto == 1) & conf[1].on){
                Caudal();                  
               } 
               on_off(texto, conf_al,0, 0);
               Pinta_Conf(texto);
               break;
     } 
   } 
 }while((ON!=12)|(modo!=2));
 while(flanco == 0){}
//se le asigna una entrada/salida fija, la número 8.
 if((conf[0].on == 1) & (conf[0].io == 9)){        //FILTRADO
  conf[0].io = 8;
  in[7].libre = 0;                     //entrada ocupada.
 } else if(conf[0].on == 0){
  in[conf[0].io-1].libre = 1;          //entrada libre.
  conf[0].io = 9;
 } 
 ON = 9;
 modo = 0;
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Si activamos Caudal y pulsamos <-| en modo 2, pasamos a configurar el 
// Caudal y la frecuencia específica del Sensor de Caudal que el usuario empleará.
// el Caudal se guardará en conf[1].io y la frecuencia para ese caudal, en frec_user.
void Caudal(void)
{  
 ON = 9;
 Limpia_Pantalla();
 Put_Str(caudal);
 Put_Str(lxmin);
 
 Put_Ins(xy26);
 Put_Num(maxcaudal);
 Put_Ins(Set_Display + Display_On + Cursor_On + Blink_On); 
 Put_Ins(xy26);
 
 conf[1].io = (int) Numero(maxcaudal, 3, xy26);
 if(ON == 22){               //modo = 2 pasa a configurar la frecuencia.
  Limpia_Pantalla();
  Put_Str(frec);
  
  Put_Ins(xy16);
  Put_Num(conf[1].io);
  
  Put_Ins(xy26);
  Put_Num(maxfrec);
  Put_Ins(xy26);  
  
  ON = 9;
  modo = 0; 
  
  frec_user = (int) Numero(maxfrec, 3, xy26);
 } 
 ON = 9;
 modo = 0;  
 Put_Ins(Set_Display + Display_On + Cursor_Off + Blink_Off);  
}

//***********************************************************************************
//ENTRADAS: texto(indica la configuración que se va ha pintar).
//SALIDAS: -
//FUNCIÓN: Muestra la configuración adjuntada en la variable 'texto' y señala si se 
// ha activado o no.
void Pinta_Conf(unsigned int texto)
{
 Limpia_Pantalla();
 switch(texto){
  case 0: Put_Ins(xy14);
          Put_Str(filtrado);           
          break;
  case 1: Put_Ins(xy15);
          Put_Str(caudal);
          break;
  case 2: Put_Ins(xy15);
          Put_Str(mot);
          break;    
  case 3: Put_Ins(xy15);//variable motor del submenú 1.Manual.
          Put_Str(mot);
          break;                
 }
 Put_Ins(LINEA2);
 Put_Str(OFF);
 Put_Ins(xy2E);
 Put_Str(OK);
 if(texto == 3){
  if((motor)&(conf[2].on==1)){
    Put_Ins(xy2D);
    Put_Str(">");
  }else{
    Put_Ins(xy23);
    Put_Str("<");
  }
 }else{   
   if (conf[texto].on==1){  //si configuración activada ,, señalizarla ">"
    Put_Ins(xy2D);
    Put_Str(">");
   }else if (conf[texto].on==0){
    Put_Ins(xy23);
    Put_Str("<");
   } 
 }     
}

//***********************************************************************************
//***********************   5.Config. IN/OUT    *************************************
//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Quinta opción del Menú. Despliega el Submenú Config. IN/OUT.
void Conf_IO(void)
{
 unsigned int sublinea = 51;

 Put_Str(menurI_O);
 Linea(sublinea);
 do{
   if (ON != 9){
    while (flanco == 0){}
     switch (ON){
      case 12: break;        // ESC
      case 3: ON = 9;        // -
                if (sublinea == 51){           
                  sublinea = 54;
                }else{ sublinea--;}
                Linea(sublinea);
                break;
      case 13: ON = 9;       // +
                if (sublinea == 54){           
                  sublinea = 51;
                }else{ sublinea++;}
                Linea(sublinea);
                break;
      case 22: ON = 9;       // <-|
                Ejecuta(sublinea);
                Limpia_Pantalla();
                Put_Str(menurI_O);
                Linea(sublinea);
                break;
     } 
   }
 }while(ON!=12);
 while (flanco == 0){}
 ON = 9;
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Se encarga de configurar las Unidades de Riego en modo LOCAL o REMOTO. 
// En modo Local, busca las posibles salidas lilbres para asociarla a esa Unidad de R.
// En modo Remoto, deja libre la salida asociada y se configura la dirección del BUS 
// en la que irá inscrita esa Unidad de Riego. Una vez que entramos en el Modo REMOTO, 
// aunque pulsemos ESC la Unidad de R. se configurará con una dirección. Para desactivar 
// este modo, activamos el modo LOCAL.
void Zonas(void)
{
 unsigned int ud = 0;        //índice del array de registros 'zona'
 unsigned int i, j;

 Pinta_Zona(ud); 
 do{
   if (ON != 9){
    while (flanco == 0){}
     switch (ON){
      case 12: if(modo == 1){// Modo LOCAL.                   
                uni[ud] = 1;
                ON = 9;                                                 
                if (zona[ud].sal == 9){   //Si venimos del Modo Remoto
                  i = 0;
                  while(salidas_ocup[i] == 0){                      
                    i++;
                  }
                  i++; 
                  zona[ud].sal = i;
                } else {
                  i = zona[ud].sal;
                }
                j = i;                    //recuperamos la antigua salida por si el usuario decide abortar (ESC).
                out[i-1].libre = 1;       //en el array de salidas libres, introducimos la salida de la zona actual como salida libre.
                                          //MAL!! y si esta salida está ocupada por el MOTOR!!!No podrá ser pq se la asignará una salida libre al MOTOR.                                                 
                i = Local(i, out, 1); 
                if(ON == 22){             //Zona configurada.
                  zona[ud].remoto = 0;
                  zona[ud].sal = i;       //actualizamos la nueva salida  
                  out[i-1].libre = 0;     //Señalamos como ocupada la nueva salida. 
                  Pinta_Ud(ud);           //Muestra la elección tomada durante unos segundos
                  Delayx100us(20000);     //espera 2seg.
                }else if (zona[ud].remoto == 0){         //si se pulsó ESC, volvemos a asignar la salida que tenía.
                  zona[ud].sal = j;
                  out[j-1].libre = 0;
                }else{                    //si se abortó, siendo la salida remota.
                  zona[ud].sal = 9;       //se vuelve a señalar como libre la salida 
                }                                       
                ON = 9;                  
                Pinta_Zona(ud);
              }
              break;                        
      case 3 : ON = 9;        // -
               if (ud == 0){                
                 ud = 5;
               }else{ ud--;}
               Pinta_Zona(ud);                            
               break;
      case 13: ON = 9;       // +
               if (ud == 5){                
                 ud = 0;
               }else{ ud++;}
               Pinta_Zona(ud);
               break;
      case 22: ON = 9;       // Modo REMOTO.
               uni[ud] = 0;  
               zona[ud].remoto = 1;          
               out[zona[ud].sal-1].libre = 1;   //en el array de registros de salida, las señalamos como libres.
               zona[ud].sal    = 9;             //salida correspondiente a esa ud estará libre.
               Remoto(); 
               zona[ud].direccion = (int) Numero(maxdir, 3, xy2B);
               ON = 9;
               
               Pinta_Ud(ud);                    //Muestra la elección tomada durante unos segundos
               Delayx100us(20000);              //espera 2seg.
               Pinta_Zona(ud);
               break;
     }  
   } 
 }while((ON!=12)|(modo!=2));                //aqui daría igual hacer una AND(&).
 while(flanco == 0){}
 ON = 9;
 modo = 0;
}   

//***********************************************************************************
//ENTRADAS: ud(Unidad de Riego a configurar).
//SALIDAS: -
//FUNCIÓN: Muestra la Unidad de Riego que vamos a configurar en modo Local o Remoto.
void Pinta_Zona(unsigned int ud)
{
 Limpia_Pantalla();
 switch(ud){
  case 0: Put_Str(unidad); Put_Str("1"); break;
  case 1: Put_Str(unidad); Put_Str("2"); break;
  case 2: Put_Str(unidad); Put_Str("3"); break;
  case 3: Put_Str(unidad); Put_Str("4"); break;
  case 4: Put_Str(unidad); Put_Str("5"); break;
  case 5: Put_Str(unidad); Put_Str("6"); break;
 }
 Put_Ins(LINEA2);
 Put_Str(local);
 Put_Ins(xy2A);
 Put_Str(remoto);
}

//***********************************************************************************
//ENTRADAS: i(índice del array inout), 
//          inout[8](array de registros out/in en el que guardamos la configuración 
//                   de cada salida/entrada), 
//          ent_sal(hace diferencia entre la configuración de Zonas/Salidas o Entradas).
//SALIDAS: i(nuevo índice del array inout).
//FUNCIÓN: Función usada para configurar las 3 opciones de este submenú: Ud Riego, 
// Salidas y Entradas.
unsigned int Local(unsigned int i, struct io inout[8], unsigned int ent_sal)
{
  Limpia_Pantalla();
  Put_Ins(xy15);
  Put_Str(local);  
     
  pinta_inout(i, ent_sal);   //pinta por defecto la salida asignada a esa Ud riego
  do{
    if (ON != 9){
      while (flanco == 0){}
      switch(ON){
        case 12: break;      // ESC
        case 3: ON = 9;      // -          
                  if(i < 2){                  
                    i = 8;
                  }else{
                    i--;
                  }  
                  while(inout[i-1].libre==0){    //buscamos las salidas libres o la zona[ud].sal
                    if (i < 2){
                      i = 8;
                    }else{ i--; }
                  }                   
                  pinta_inout(i, ent_sal); 
	          break;
        case 13: ON = 9;     // +
                  if(i > 7){
                    i = 1;
                  }else{
                    i++;
                  }    
                  while(inout[i-1].libre==0){   
                    if (i > 7){
                      i = 1;
                    }else{ i++; }
                  }
                  pinta_inout(i, ent_sal);
                  break;
        case 22: break;      //<-|     Este caso lo estudiamos al terminar esta función, en Zonas() o Salidas().
      }//switch
    }//if  
  }while((ON!=12)&(ON!=22)); //do      Sale del programa con botón ESC o cuando pulsamos <-|
  while(flanco == 0){}
 return(i);
}

//***********************************************************************************
//ENTRADAS: num(salida o entrada libre encontrada), 
//          ent_sal(hace diferencia entre la configuración de Zonas/Salidas o Entradas).
//SALIDAS: -
//FUNCIÓN: Pinta una de las salidas libres encontradas.
 void pinta_inout(unsigned int num, unsigned int ent_sal)
{
  if((ent_sal == 1) | (ent_sal == 12)){
    Put_Ins(xy24);
    Put_Str(sal); 
  }else{ 
    Put_Ins(xy23);
    Put_Str(ent);
  }  
  Put_Num(num);

  Put_Ins(xy2B);  
  Put_Ins (Set_Display +Display_On + Cursor_On + Blink_On);
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Prepara la pantalla para que el usuario introduzca la direcciona asociada 
// a la Unidad de Riego/Salida o Entrada.
void Remoto(void) 
{
 Limpia_Pantalla();
 Put_Ins(xy15);
 Put_Str(remoto);
 
 Put_Ins(xy21);
 Put_Str(direc);

 Put_Num(maxdir); 
 
 Put_Ins(xy2B);
 Put_Ins (Set_Display + Display_On + Cursor_On + Blink_On);  
}

//***********************************************************************************
//ENTRADAS: ud(Unidad de Riego que se va a pintar).
//SALIDAS: -
//FUNCIÓN: Muestra la Unidad de Riego ya configurada en modo Local o Remoto.
void Pinta_Ud(unsigned int ud)      
{
 Put_Ins (Set_Display + Display_On + Cursor_Off + Blink_Off); 
 Pinta_Zona(ud);
 Limpia_Linea(LINEA2);
 if(zona[ud].on){
   if (zona[ud].remoto==0){    //Modo LOCAL
    Put_Ins(xy21);
    Put_Str(local);
    Put_Ins(xy27);
    Put_Str(sal);  
    Put_Num(zona[ud].sal);  
   } else {                    //Modo REMOTO
    Put_Ins(xy23);       
    Put_Str(remoto);
    Put_Ins(xy2B);
    Put_Num(zona[ud].direccion); 
   }
 }else{
   Put_Ins(xy22);
   Put_Str(OFF);
   Put_Ins(xy27);
   Put_Str(sal);  
   Put_Num(zona[ud].sal);     
 }  
}

//***********************************************************************************
//ENTRADAS: ent_sal_ocup(distingue entre salida ocupada/libre o entrada ocupada/libre),
//          inout[8](array de registros out/in en el que guardamos la configuración 
//                   de cada salida/entrada),
//          array_ocup[9](array de out/in que indica la configuración de cada out/in, 
//                        y el último elemento indica si todas las inout está libres
//                        o no), 
//          ud(Unidad de Riego).
//SALIDAS: -
//FUNCIÓN: Función usada para la configuración tando de las Salidas, como de las Entradas
// en Modo LOCAL o REMOTO.
void IN_OUT(unsigned int ent_sal_ocup, struct io in_out[8], unsigned int array_ocup[9])
{
 unsigned int i = 0, j, k, h = 0;
 
 while(in_out[i].libre==0){  //Pinta la primera salida libre.
  i++;
  if (i==8){
    array_ocup[8] = 0;       //Todas las io ocupadas.    
    i = 0;
    break;
  }  
 } 
 i++;
 j = i; 
 pinta_io(i, ent_sal_ocup);
 
 for(k=0; k<7; k++){         //copia de out en ocupada
  array_ocup[k] = in_out[k].libre;     
 } 
 if((array_ocup[8] == 0) & (ent_sal_ocup == 0)){      //si hay alguna entrada libre
  ent_sal_ocup = 2;                                   //Pinta Entrada ocupada  
 }else if ((array_ocup[8] == 0) & (ent_sal_ocup == 1)){
  ent_sal_ocup = 12;                                  //Pinta Salida ocupada  
 } 
 pinta_io(i, ent_sal_ocup);
  
 do{
   if (ON != 9){
    while (flanco == 0){}
     switch (ON){
      case 12: if((modo == 1) & (array_ocup[8]==1)){  //Modo LOCAL    
                  ON = 9;
                  i = Local(i, in_out, ent_sal_ocup); //ent_sal_ocup sólo puede valer 0ó1
                  if(ON == 22){                                          
                    in_out[i-1].libre = 0;            //i es la io que acabamos de configurar.
                    Pinta_OutIn(i, ent_sal_ocup);     //Muestra la elección tomada durante unos segundos//ent_sal_ocup sólo puede valer 0ó1
                    Delayx100us(20000);               //espera 2seg.
                  }else{
                    in_out[j-1].libre = 0;            //Si abortamos(12)la antigua salida estará ocupada.
                    in_out[i-1].libre = 1;            //y la nueva volverá a estar libre.
                  }  
                  ON = 13;
                  h = 10;    //para advertir en 13 que acabamos de salir de aki
                }else if(modo == 2){}                 //Si boton 12 pulsado en modo 2,,vuelvo a menu
                break;                        
      case 3: ON = 9;                 
                  if(i < 2){                  
                    i = 8;
                  }else{
                    i--;
                  }
                  k = 0;
                  while(in_out[i-1].libre==0){        //buscamos la salida libre.
                    if (i < 2){
                      i = 8;
                    }else{ i--; }
                  }
                  pinta_io(i, ent_sal_ocup);
	          break;
      case 13: ON = 9; 
                if(i > 7){
                  i = 1;
                }else{
                  i++;
                }
                k = 0;
                while((in_out[i-1].libre==0)){   
                  if (i > 7){
                    i = 1;
                  }else{ i++; }
                  k++;
                  if(k == 8){                         //Si todas las salidas enstán ocupadas, sale del while
                    array_ocup[8] = 0;                //Todas las salidas ocupadas. 
                    if(ent_sal_ocup==0){
                      ent_sal_ocup = 2;               //Pinta Entrada ocupada
                    }else if (ent_sal_ocup==1){   
                      ent_sal_ocup = 12;              //Pinta Salida ocupada
                    }  
                    for(j=0; j<8; j++){               //copia de ocupada en in_out
                     if (h != 10){
                      in_out[j].libre = array_ocup[j];
                       if (in_out[j-1].libre == 1){
                        i = j;                        //el índice apunta a la salida que antes estaba libre. AKI NO ENTRA!!! 
                      }
                     }   
                    }                      
                    break;      
                  }   
                }
                pinta_io(i, ent_sal_ocup);
                break;
      case 22: ON = 9;
                in_out[i-1].remoto = 1;     //Modo REMOTO 
                in_out[i-1].libre = 1;      //en el array de registros de salida, las señalamos como libres. 
                
                array_ocup[8] = 1;          //ya hay al menos una salida libre.                  
                Remoto();                     
                in_out[i-1].direccion = (int) Numero(maxdir, 3, xy2B);    //valor entero de la direccion del dispositivo direccionado en Modo Remoto.
                ON = 9;
                Pinta_OutIn(i, ent_sal_ocup);         //Muestra la elección tomada durante unos segundos
                Delayx100us(20000);                   //espera 2seg.
                pinta_io(i, ent_sal_ocup);
                break;
      }  
   } 
 }while((ON!=12)|(modo!=2)); //aqui daría igual hacer una AND(&).
 while(flanco == 0){}
 ON = 9;
 modo = 0;
 OnOff_Rele();
}

//***********************************************************************************
//ENTRADAS: num(out/in que antes de configurarla estaba libre), 
//          ent_sal(distingue entre salida ocupada/libre o entrada ocupada/libre).
//SALIDAS: -
//FUNCIÓN: Avisa que todas las out/in estan ocupadas, pero indica las out 
// ocupadas(LOCAL) que no han sido configuradas como unindad de Riego, para tener la 
// oportunidad de volverlas a liberar, configurándola en Modo Remoto.
void pinta_io(unsigned int num, unsigned int ent_sal)
{
 Limpia_Pantalla();
 if(ent_sal == 0){
  Put_Str(entrada);
 }else if (ent_sal == 1){
  Put_Str(salida);
 }else if (ent_sal == 2){
  Put_Str(ent_ocup);
  Put_Ins(xy27); 
 }else{            //ent_sal == 12
  Put_Str(sal_ocup);
  Put_Ins(xy27); 
 }    
 
 Put_Num(num);             
 Put_Ins(LINEA2);
 Put_Str(local);
 Put_Ins(xy2A);
 Put_Str(remoto);
}

//***********************************************************************************
//ENTRADAS: i(índice del array out/in[]), 
//          ent_sal(distingue entre salida o entrada).
//SALIDAS: -
//FUNCIÓN: Muestra la asignación configurada. 
void Pinta_OutIn(unsigned int i, unsigned int ent_sal)
{  
 Put_Ins (Set_Display + Display_On + Cursor_Off + Blink_Off); 
 Limpia_Pantalla();
 if((ent_sal==0) | (ent_sal==2)){
  Put_Str(entrada);
 }else if ((ent_sal==1) | (ent_sal==12)){
  Put_Str(salida);
 }
 Put_Num(i);
 Put_Ins(xy22);
 Put_Str(configurada);  
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Se organiza en un Submenú una visualización de las Unidades de Riego, Salidas
// y entradas configuradas.
void Ver_Conf(void)
{
 unsigned int sublinea = 541;

 Linea(sublinea);
 do{
   if (ON != 9){
    while (flanco == 0){}
     switch (ON){
      case 12: break;        // ESC
      case 3: ON = 9;        // -
                if (sublinea == 541){   
                  sublinea = 543;
                }else{ sublinea--;}
                Linea(sublinea);
                break;
      case 13: ON = 9;        // +
                if (sublinea == 543){  
                  sublinea = 541;
                }else{ sublinea++;}
                Linea(sublinea);
                break;
      case 22: ON = 9;        // <-|
                ejecuta_ver(sublinea);
                Limpia_Pantalla();
                Linea(sublinea);
                break;
     } 
   }
 }while(ON!=12);
 while(flanco == 0){}
 ON = 9;
}

//***********************************************************************************
//ENTRADAS: sublinea(distingue entre Unidades de Riego, Salidas y Entradas).
//SALIDAS: -
//FUNCIÓN: Según sea sublínea UD, Salidas o Entradas, muestra la configuración de 
// cada una de ellas.
void ejecuta_ver(unsigned int sublinea)
{
 unsigned int i = 1, j = 1, k = 1;
 
 switch(sublinea){
  case 541: Pinta_Ud(i-1);
            do{
             if (ON != 9){
              while (flanco == 0){}
               switch (ON){
                case 12: break;   // ESC
                case 3: ON = 9;   // -
                          if (i == 1){  
                            i = 6;
                          }else{ i--;}
                          Pinta_Ud(i-1);
                          break;
                case 13: ON = 9;  // +
                          if (i == 6){ 
                            i = 1;
                          }else{ i++;}
                          Pinta_Ud(i-1);
                          break;
                case 22: break;   // <-|
               } 
             }
            }while((ON!=12) & (ON!=22));
            while(flanco == 0){}
            ON = 9;          
            break;
  case 542: Pinta_reg(j, out, 1);
            do{
             if (ON != 9){
              while (flanco == 0){}
               switch (ON){
                case 12: break;   // ESC
                case 3: ON = 9;   // -
                          if (j == 1){     
                            j = 8;
                          }else{ j--;}
                          Pinta_reg(j, out, 1);
                          break;
                case 13: ON = 9;  // +
                          if (j == 8){  
                            j = 1;
                          }else{ j++;}
                          Pinta_reg(j, out, 1);
                          break;
                case 22: break;   // <-|
               } 
             }
            }while((ON!=12) & (ON!=22));
            while(flanco == 0){}
            ON = 9;
            break;
  case 543: Pinta_reg(k, in, 0); 
            do{
             if (ON != 9){
              while (flanco == 0){}
               switch (ON){
                case 12: break;   // ESC
                case 3: ON = 9;   // -
                          if (k == 1){     
                            k = 8;
                          }else{ k--;}
                          Pinta_reg(k, in, 0);
                          break;
                case 13: ON = 9;  // +
                          if (k == 8){     
                            k = 1;
                          }else{ k++;}
                          Pinta_reg(k, in, 0);
                          break;
                case 22: break;   // <-|
               } 
             }
            }while((ON!=12) & (ON!=22));
            while(flanco == 0){}
            ON = 9;
            break;
 }
}

//***********************************************************************************
//ENTRADAS: i(índice del array de registros in/out), 
//          inout[8](array de registros tipo io del que escribiremos la configuración
//                   de las entradas o salidas),
//          ent_sal(distingue entre salida o entrada).
//SALIDAS: -
//FUNCIÓN: Muestra la configuración seleccionada según el array inout.
void Pinta_reg(unsigned int i, struct io inout[8], unsigned int ent_sal)
{
 Limpia_Pantalla();
 if(ent_sal == 1){
   Put_Str(salida); 
 }else{ 
   Put_Str(entrada);
 }  
 
 Put_Num(i);
 if (inout[i-1].libre == 1){
  
  if (inout[i-1].remoto == 0){    //inout libre y en modo local, xq en modo remoto tb deja la inout libre
    Put_Ins(xy25);
    Put_Str(libre);
  }else{                          //Modo REMOTO
    Put_Ins(xy23);   
    Put_Str(remoto);
    Put_Ins(xy2B);
    Put_Num(inout[i-1].direccion);       
  }
  
 }else{
  
  if (inout[i-1].remoto == 0){    //Modo Local y inout ocupada
    Put_Ins(xy22);
    Put_Str(local); 
    Put_Ins(xy29);    
   
   if((conf[0].on = 1) & (conf[0].io == i) & (ent_sal == 0)){          //Filtrado se ha activado y se le asigna la entrada i       
     Put_Str(filtro);
   }else if ((conf[2].on = 1) & (conf[2].io == i) & (ent_sal == 1)){   //Motos se ha activado y se le asigna la salida i 
    Put_Str(mot);
   }else{
    Limpia_Linea(LINEA2);
    Put_Ins(xy25);
    Put_Str(local);    
   } 
   
  }else{
   Put_Ins(LINEA2); 
   Put_Str(remoto);
   Put_Ins(xy27);
   Put_Num(inout[i-1].direccion);     
   if((conf[0].on = 1) & (conf[0].io == i) & (ent_sal == 0)){//Filtrado se ha activado y entra por la entrada i q tb es remota.
    Put_Ins(xy2A);
    Put_Str(filtro);
   }else if ((conf[2].on = 1) & (conf[2].io == i) & (ent_sal == 1)){      
    Put_Ins(xy2B);    
    Put_Str(mot);
   }
       
  }
 }       
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Activa y desactiva los relés a través del array de registros 'out'.
void OnOff_Rele(void)
{
 unsigned int i;

 if(bitest(P4IN,BIT7)){ //Si no está filtrando.Opto activo con ceros.
   for(i=0; i<8; i++){
    if(!out[i].libre){  // & (!out[i].remoto)){//La salida puede estar en modo Remoto y estar libre!!
      bitset(RELE_OUT, i);
    }else{
      bitclr(RELE_OUT, i);
    }
   };
 }  
}

//***********************************************************************************
//***********************   6.Config. Alarma    *************************************
//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Sexta opción del Menú. Se encarga de configurar o no las 4 alarmas 
// posibles que se pueden generar.
void Conf_AL(void)
{
 unsigned int texto = 0;
 unsigned int conf_al = 1;

 Pinta_Alarma(texto);
 do{
   if (ON != 9){
    while (flanco == 0){}
     switch (ON){
       case 12: if(modo == 1){
                   on_off(texto, conf_al,0, 0);
                   Pinta_Alarma(texto);
                  }else if(modo == 2){}           //Si boton 12 pulsado en modo 2,,vuelvo a menu
                  break;                          // NO
        case 3: ON = 9;
                  if (texto == 0){               //arriba
                    texto = 3;
                  }else{ texto--;}
                  Pinta_Alarma(texto);                             
                  break;
        case 13: ON = 9;
                  if (texto == 3){               // abajo
                    texto = 0;
                  }else{ texto++;}
                  Pinta_Alarma(texto);                
                  break;
        case 22: on_off(texto, conf_al,0, 0);
                  Pinta_Alarma(texto);
                  break;
       }  
   } 
 }while(modo!=2);              
 while(flanco == 0){}
 ON = 9;
 modo = 0;
}

//***********************************************************************************
//ENTRADAS: texto(distingue entre las 4 posibles alarmas).
//SALIDAS: -
//FUNCIÓN: Muestra si está activada o no la alarma vinculada a la variable 'texto'
void Pinta_Alarma(unsigned int texto)
{
 Limpia_Pantalla();
 Put_Ins(xy14);
 switch(texto){
  case 0: Put_Str(filtro);           
           break;
  case 1: Put_Str(dep);
          Put_Ins(xy24);
          Put_Str(rebosado);
          break;
  case 2: Put_Str(dep);
          Put_Ins(xy26);
          Put_Str(seco);
          break;
  case 3: Put_Str(error);
          Put_Ins(xy24);
          Put_Str(comunic);
          break;
 }
 Put_Ins(LINEA2);
 Put_Str(OFF);
 Put_Ins(xy2E);
 Put_Str(OK);
 if (alarma[texto].on==1){              //si alarma activada ,, señalizarla ">"
    Put_Ins(xy2D);
    Put_Str(">");
 } else if (alarma[texto].on==0){
    Put_Ins(xy23);
    Put_Str("<");
 }  
}

//***********************************************************************************
//ENTRADAS: texto(índice del array de registros, cuyos campos serán activados o no),
//          conf_al_ud(distingue entre 3 arrays: conf[], alarma[] y zona[]/ud_off/ud_on),
//          i(índice del array programa),
//          sublinea(distingue entre los arrays zona[], programa[i].ud_on, 
//                   programa[i].ud_off y conf[2].on <- motor).
//SALIDAS: -
//FUNCIÓN: Capta el valor del botón pulsado para luego llamar a la función si_no(),
// si se pulsa ESC ó <-|
void on_off(unsigned int texto, unsigned int conf_al_ud, unsigned int i, unsigned int sublinea)
{
    switch (ON){
      case 12: ON = 9;
               si_no(1, texto, conf_al_ud, i, sublinea);
               break;
      case 3 : break;
      case 13: break;
      case 22: ON = 9;
               si_no(0, texto, conf_al_ud, i, sublinea);
               break;
    }
}

//***********************************************************************************
//ENTRADAS: sn(distingue si se ha pulsado ESC(OFF) ó <-|(ON)),
//          texto(índice del array de registros, cuyos campos serán activados o no),
//          conf_al_ud(distingue entre 3 arrays: conf[], alarma[] y zona[]/ud_off/ud_on),
//          i(índice del array programa),
//          sublinea(distingue entre los arrays zona[], programa[i].ud_on, 
//                   programa[i].ud_off y conf[2].on <- motor).
//SALIDAS: -
//FUNCIÓN: Programa usado por 4.Configuración, 6.Conf. Alarmas, por 2.Programa Riego 
// y por 1.Manual, para la activacion o desactivación de configuración, alarmas, Unidades
// de Riego, o Motor.
void si_no(unsigned int sn, unsigned int texto, unsigned int conf_al_ud, unsigned int i, unsigned int sublinea)
{                                                                         
 Limpia_Pantalla();                                                       
 if(sn == 0){
   Put_Ins(xy14);
   Put_Str(PREGA);
   Put_Ins(LINEA2);
   Put_Str(SI);
   Put_Ins(xy2E);
   Put_Str(NO);
 }else{
   Put_Ins(xy12);
   Put_Str(PREGD);
   Put_Ins(LINEA2);
   Put_Str(NO);
   Put_Ins(xy2E);
   Put_Str(SI);   
 }

 do{
  if (ON != 9){
   Limpia_Pantalla();
   while (flanco == 0){}    
    switch (ON){
      case 12: if(conf_al_ud == 0){        
                  conf[texto].on = 1;
                  conf[2].io = 8;
                  Put_Str(A);
                }else if (conf_al_ud == 1){      
                  alarma[texto].on = 1;
                  Put_Str(A);                  
                }else if (sublinea==21){    //sublinea ==21 ,, hora_on
                  programa[i-1].ud_on[texto] = 1;
                  Put_Str(A);
                }else if (sublinea==22){    //sublinea == 22 ,, hora_off/Litros off
                  programa[i-1].ud_off[texto] = 1;
                }else if (sublinea == 11){
                  zona[texto].on = 1;
                  Put_Str(A);
                }else if (sublinea == 12){  // Motor on en 1.Manual sólo si está configurado
                  if(conf[2].on){
                    motor = 1;                    
                    out[7].libre = 0;         //salida ocupada.
                    bitset(RELE_OUT, 7); 
                    Put_Str(A);
                  }
                }                                  
                ON = 3;               
                break;
      case 3: break;          
      case 13: break;
      case 22: if(conf_al_ud == 0){
                  conf[texto].on = 0;
                  motor = 0;
                  out[conf[2].io-1].libre = 1;   //salida libre.                    
                  conf[2].io = 9;
                  Put_Str(D);
                }else if (conf_al_ud == 1){    
                  alarma[texto].on = 0;
                  Put_Str(D);                  
                }else if (sublinea==21){
                  programa[i-1].ud_on[texto] = 0;
                }else if (sublinea==22){
                  programa[i-1].ud_off[texto] = 0;
                  Put_Str(D);
                }else if (sublinea == 11){
                  zona[texto].on = 0;
                  Put_Str(D);
                }else if (sublinea == 12){
                  if((conf[2].on)&(motor)){
                    motor = 0;
                    out[7].libre = 1;   //salida libre. 
                    bitclr(RELE_OUT, 7);                    
                    Put_Str(D);
                  }  
                }                                
                ON = 3;               
                break;
    } 
  }
 }while((ON!=3)&(ON!=13));
 Delayx100us(10000);        //espera 1seg.  
 ON = 9;
 modo = 0;
} 

//***********************************************************************************
//***********************        7.Config. Reloj         ****************************
//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Séptima opción del Menú. Despliega dos elecciones de configuración:
// Hora y Fecha.
void Reloj(void)
{
 unsigned int sublinea = 71;

 Put_Str(menureloj);
 Linea(sublinea);

 do{
   if (ON != 9){
    while (flanco == 0){}
     switch (ON){
      case 12: break;                           // NO
      case 3: ON = 9;
                modo = 0;
                if (sublinea == 71){             //arriba
                  sublinea = 72;
                }else{ sublinea--;}
                Linea(sublinea);
                break;
      case 13: ON = 9;
                modo = 0;
                if (sublinea == 72){             // abajo
                  sublinea = 71;
                }else{ sublinea++;}
                Linea(sublinea);
                break;
      case 22: ON = 9;                        //ok
                Ejecuta(sublinea);
                Limpia_Pantalla();
                Put_Str(menureloj);
                Linea(sublinea);
                break;
     }
   } 
 }while(ON!=12);
 ON = 9;
}

//***********************************************************************************
//ENTRADAS: hora(distingue quién llamó a ésta funcion: desconexión de zona[ud], llenado
//               del depósito, hora_on y hora_off del programa de riego, hora de 
//               desconexión del motor y para actualizar la hora),
//          i(índice del array de programas),
//          ud(índice del array de registros 'zona').
//SALIDAS: -
//FUNCIÓN: Función usada por 6 programas distintos que son diferenciados por la variable
// hora. Se guarda en la variable correspondiente la Hora configurada.
void Poner_Hora(unsigned int hora, unsigned int i, unsigned int ud)                              
{                                  
  Put_Ins(LINEA1);                
  Put_Str(hh);                     
  Put_Ins(LINEA2);
  
  if (hora == 0){                           //si hora == 0, fija hora de desconexion para zona[ud]
    horas = zona[ud].hora_off.hr;
    minutos = zona[ud].hora_off.min;
  }else if (hora == 1){                     //si hora == 1, fija hora de llenado del depósito
    horas = hora_llenado.hr;
    minutos = hora_llenado.min;
  }else if (hora == 2){                     //si hora == 2, hora_on del programa de riego
    horas = programa[i-1].hora_on.hr;
    minutos = programa[i-1].hora_on.min;
  }else if (hora == 3){                     //si hora == 3, hora_off del programa de riego
    horas = programa[i-1].hora_off.hr;
    minutos = programa[i-1].hora_off.min;
  }else if (hora == 4){                     //si hora == 4, motor_off 
    horas = motor_off.hr;
    minutos = motor_off.min;
  }else if (hora == 5){                     //actualiza hora
    horas = date.hora.hr;
    minutos = date.hora.min;
  }
  Hora(minutos, horas);
  Put_Ins(xy22);
  Put_Ins(Set_Display +Display_On + Cursor_On + Blink_On);

 do{
  if (ON != 9){
   while (flanco == 0){}
   switch(ON){
    case 12: if (modo == 1){
               modo = 2;
             }  
             break;
    case 13: ON = 9;
             modo = 0;
             horas++;
             if (horas == 24) {horas = 0;}
             Put_Ins(LINEA2);
             Hora(minutos, horas);
             Put_Ins(xy22);
             break;
    case 3 : ON = 9; 
             modo = 0;
             horas--;
             if (horas < 0) { horas = 23;}	
             Put_Ins(LINEA2);
             Hora(minutos, horas);
             Put_Ins(xy22);
             break;
    case 22: if (modo == 1){
               ON = 9;
               actualiza_mm(0);   //hora actualizada, ahora los minutos
               modo = 0;
               Put_Ins(xy22);
             }
             break; 
   }
  }
 }while (modo!=2);  
 while (flanco == 0){}
 
 if (hora == 0){                            //si hora == 0, fija hora de desconexion para zona[ud]
  zona[ud].hora_off.hr = horas;
  zona[ud].hora_off.min = minutos;
 }else if (hora == 1){                      //si hora == 1, fija hora de llenado del depósito
  hora_llenado.hr = horas;
  hora_llenado.min = minutos;
 }else if (hora == 2){                      //si hora == 2, hora_on del programa de riego
  programa[i-1].hora_on.hr = horas;
  programa[i-1].hora_on.min = minutos;
 }else if (hora == 3){                      //si hora == 3, hora_off del programa de riego
  programa[i-1].hora_off.hr = horas;
  programa[i-1].hora_off.min = minutos;
 }else if (hora == 4){                      //si hora == 4, motor_off 
    motor_off.hr = horas;
    motor_off.min = minutos;
 }else if (hora == 5){                      //actualiza hora
    date.hora.hr = horas;
    date.hora.min = minutos;
    date.hora.seg = 0;
 }     
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Actualiza la Fecha introducida por el usuario.
void Poner_Fecha(void)
{
  Put_Ins(LINEA1);
  Put_Str(ff);
  Put_Ins(LINEA2);    
  Fecha(date.fecha.dd, date.fecha.mes, date.fecha.aa);
  Put_Ins(xy22);
  Put_Ins(Set_Display +Display_On + Cursor_On + Blink_On);
 do{
  if (ON != 9){
   while (flanco == 0){}
   switch(ON){
    case 12: if (modo == 1){
                modo = 2;
              }  
              break;
    case 13: ON = 9;
              modo = 0;
              if ((date.fecha.dd<31)&(mes_impar|(date.fecha.mes==12))|((date.fecha.dd<30)&mes_par)|((date.fecha.dd<29)&(date.fecha.mes==2)&(date.fecha.aa%4 == 0))|((date.fecha.dd<28)&(date.fecha.mes==2)&(date.fecha.aa%4 != 0))) {
              	Put_Ins(LINEA2);
              	date.fecha.dd++;
              	Fecha(date.fecha.dd, date.fecha.mes, date.fecha.aa);
              	Put_Ins(xy22);              	
              }
              break;
    case 3: ON = 9;
              modo = 0;
              if (date.fecha.dd!= 1){
                date.fecha.dd--;
              	Put_Ins(LINEA2);
              	Fecha(date.fecha.dd, date.fecha.mes, date.fecha.aa);
              	Put_Ins(xy22);
              }
              break;
    case 22: ON = 9;
              actualiza_mm(1); //dia actualizado, ahora los meses
              modo = 0;
              Put_Ins(xy22);
              break;
   }
  }
 }while(modo != 2);      
 while (flanco == 0){} 
}

//***********************************************************************************
//ENTRADAS: min_mes(distingue entre minutos y meses).
//SALIDAS: -
//FUNCIÓN: Según la entrada min_mes, se actualizarán los meses o los minutos
void actualiza_mm(unsigned int min_mes) 
{
 //// si min_mes = 0 ,, minutos   si  min_mes = 1 ,, mes
 Put_Ins(xy25);
 do{
  if (ON != 9){
   while (flanco == 0){}
   switch(ON){
    case 12: if (modo == 1){
                modo = 2;
              }  
              break;
    case 13: ON = 9;
              modo = 0;
              if(!min_mes){
                minutos++;
                if (minutos == 60) { minutos = 0;}
              }else{
                date.fecha.mes++;
                if (date.fecha.mes == 13){ date.fecha.mes = 1;}
              }
              break;
    case 3: ON = 9;
              modo = 0;
              if(!min_mes){
                minutos--;
                if (minutos < 0) { minutos = 59;}
              }else{
                date.fecha.mes--;
                if (date.fecha.mes == 0) {date.fecha.mes = 12;}
              }
              break;
    case 22: if(!min_mes){
                modo = 2;                
              }else{
                ON = 9;
                modo = 0;
                actualiza_aa();
              }
              break;
   }
   Put_Ins(LINEA2);
   if(!min_mes){
    Hora(minutos, horas);
   }else{
    if((date.fecha.mes==2)&(date.fecha.aa%4 == 0)){ date.fecha.dd = 29;
    }else if((date.fecha.mes==2)&(date.fecha.aa%4 != 0)){date.fecha.dd = 28;}
    Fecha(date.fecha.dd, date.fecha.mes, date.fecha.aa);
   }
   Put_Ins(xy25);
  }
 }while (modo != 2);             
 while(flanco == 0){}
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Restablece el valor de los años introducidos por el usuario para actualizar
// la Fecha.
void actualiza_aa(void)
{
 Put_Ins(xy28);
 do{
  if (ON != 9){
   while (flanco == 0){}
   switch(ON){
    case 12: if (modo == 1){
                modo = 2;
              }  
              break;
    case 3: ON = 9;
              modo = 0;
              date.fecha.aa++;
              if (date.fecha.aa == 100){ date.fecha.aa = 0;}
              break;
    case 13: ON = 9;
              modo = 0;
              date.fecha.aa--;
              if (date.fecha.aa == 0) {date.fecha.aa = 99;}
              break;
    case 22: ON = 9;
              modo = 2;              
              break;
   }
   if((date.fecha.mes==2)&(date.fecha.aa%4 == 0)){ date.fecha.dd = 29;
   }else if((date.fecha.mes==2)&(date.fecha.aa%4 != 0)){date.fecha.dd = 28;}
   Put_Ins(LINEA2);
   Fecha(date.fecha.dd, date.fecha.mes, date.fecha.aa);
   Put_Ins(xy28);
  }
 }while(modo != 2);
 while(flanco == 0){}
}

//***********************************************************************************
//ENTRADAS: min(minutos que debe pintar),
//          hr(horas que debe pintar).
//SALIDAS: -
//FUNCIÓN: Muestra por pantalla la Hora(horas y minutos) introducida por valor.
void Hora(int min, int hr)
{
 unsigned int MSBMIN, MSBHR, LSBMIN, LSBHR;
 char LCD[16];
 
  MSBMIN = min/10;      
  LSBMIN = min % 10;    //resto
  MSBHR = hr/10;
  LSBHR = hr % 10;

  sprintf(LCD, " %d%d:%d%d ", MSBHR, LSBHR, MSBMIN, LSBMIN);
  Put_Str(LCD);
}

//***********************************************************************************
//ENTRADAS: dd(dia que debe pintar),
//          mes(mes que debe pintar),
//          aa(año que debe pintar).
//SALIDAS: -
//FUNCIÓN: Muestra por pantalla la Fecha(dia, mes y año) que le llegan como entrada.
void Fecha(unsigned int dd,//struct fecha f
           unsigned int mes,
           unsigned int aa)
{
 unsigned int MSBDD, MSBMES, MSBAA, LSBDD, LSBMES, LSBAA;
 char LCD[16];

  MSBDD = dd/10;
  LSBDD = dd % 10;
  MSBMES = mes/10;
  LSBMES = mes % 10;
  MSBAA = aa/10;
  LSBAA = aa % 10;

  sprintf(LCD, " %d%d/%d%d/%d%d", MSBDD, LSBDD, MSBMES, LSBMES, MSBAA, LSBAA);
  Put_Str(LCD);
}

//***********************************************************************************
//***********************          8.Deposito         *******************************
//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN:  Octava opción del Menú. Permite configurar la hora de llenado del 
//depósito, el porcentaje mínimo de llenado, y el porcentaje mínimo de llenado en riego.
void Deposito(void)
{
 unsigned int sublinea = 81;

 Put_Str(menudep);
 Linea(sublinea);
 do{
   if (ON != 9){
    while (flanco == 0){}
     switch (ON){
      case 12: break;                           // NO
      case 3: ON = 9;
                if (sublinea == 81){             //arriba
                  sublinea = 83;
                }else{ sublinea--;}
                Linea(sublinea);
                break;
      case 13: ON = 9;
                if (sublinea == 83){             // abajo
                  sublinea = 81;
                }else{ sublinea++;}
                Linea(sublinea);
                break;
      case 22: ON = 9;                        //ok
                Ejecuta(sublinea);
                Limpia_Pantalla();
                Put_Str(menudep);
                Linea(sublinea);
                break;
     }
   }
 }while(ON!=12);
 while (flanco == 0){}
 ON = 9;
}
//***********************************************************************************
//ENTRADAS: llenado_riego(distingue entre el %de llenado y el %de llenado en riego).
//SALIDAS: -
//FUNCIÓN: Diferencia entre los dos posibles porcentajes mínimos de llenado, que 
// serán comparados en la RTI para el sensor de Nivel.
void Porcentaje(unsigned int llenado_riego) 
{                       
  int porcentaje;
  
  Put_Ins(LINEA1);
  Put_Str(porciento);
  Put_Ins(LINEA2);
  if(llenado_riego == 0){ 
    Put_Str(lleno);
  }else{
    Put_Str(regando);
  }  
  Put_Ins(xy29);
  Put_Num(maxporc);          
  Put_Ins(xy29);
  Put_Ins (Set_Display +Display_On + Cursor_On + Blink_On);

  porcentaje = (int) Numero(maxporc, 3, xy29);
                    
 if(llenado_riego == 0){ 
  porcentaje_llenado = porcentaje;    
 }else{
  porcentaje_riego = porcentaje;
 } 
}

//***********************************************************************************
//***********************          9.Guarda Conf.        ****************************
//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Novena opción del Menú. Se encarga de dar la posibilidad al usuario de 
// guardar o no la configuración ya seleccionada en Memoria Flash, para no perderla 
// si ocurriera una pérdida de alimentación, tanto de la batería como de la principal.
void Guarda_Conf(void)
{
 Put_Str(PREGC);   //escribe: Guardar?
 Put_Ins(LINEA2);
 Put_Str(NO);
 Put_Ins(xy2E);
 Put_Str(SI);
 do{
   if (ON != 9){
    while (flanco == 0){} 
      if(ON == 22){
        Limpia_Pantalla();
        Put_Str(guardada);
        //Guarda en Flash conf[3], in[8], out[8], zona[6], hora_llenado
        //porcentaje_llenado, porcentaje_riego y por último alarma[4]que no será dinámica.
        Write_Flash();
        ON = 12;
        Delayx100us(20000);  //espera 2seg. 
      }
   } 
 }while(ON!=12);
 while(flanco == 0){}
 ON = 9;
 modo = 0; 
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Función llamada cuando el usuario eligió Guardar la configuración en Memoria
// Flash.
void Write_Flash(void)
{
 char *Flash_ptr;  // Flash pointer
 unsigned int i, j;

 Flash_ptr = (char *) 0x1000; //Se posiciona el puntero en el inicio del Segmento B.

 _DINT();
 FCTL2 = FWKEY + FSSEL_2 + FN5;   // SMCLK/32 = 250kHz para Flash Timing Generator
 FCTL3 = FWKEY;                   // Limpia el bit Lock.
 FCTL1 = FWKEY + ERASE;           // Ponemos a 1 el bit de borrado
 *Flash_ptr = 0;                  // escritura Dummy para borrar el segmento de la Flach.
 
 FCTL1 = FWKEY + WRT;             // operación de escritura.
 
 //Escribir conf[], io[], zona[], alarma[],...
 
 for (i=0; i<3; i++){
  *Flash_ptr++ = conf[i].on;
  *Flash_ptr++ = conf[i].io;
 }
 for (i=0; i<8; i++){
  *Flash_ptr++ = in[i].libre;
  *Flash_ptr++ = in[i].remoto;
  *Flash_ptr++ = in[i].direccion;
  *Flash_ptr++;
 }
 for (i=0; i<8; i++){
  *Flash_ptr++ = out[i].libre;
  *Flash_ptr++ = out[i].remoto;
  *Flash_ptr++ = out[i].direccion;
  *Flash_ptr++;
 }
 for (i=0; i<6; i++){
  *Flash_ptr++ = zona[i].on;
  *Flash_ptr++ = zona[i].desconex;
  *Flash_ptr++ = zona[i].remoto;
  *Flash_ptr++ = zona[i].sal;
  *Flash_ptr++ = zona[i].direccion;   //ocupa 1 byte, pero dejo libre el otro byte 
  *Flash_ptr++;                       //para q empiece en direccion par.
  *Flash_ptr++ = zona[i].hora_off.hr;
  *Flash_ptr++ = zona[i].hora_off.min;  
 } 
 for (i=0; i<4; i++){
  *Flash_ptr++ = alarma[i].on;
  *Flash_ptr++ = alarma[i].num_alarmas;
  *Flash_ptr++ = alarma[i].hora.hr;
  *Flash_ptr++ = alarma[i].hora.min;
  *Flash_ptr++ = alarma[i].fecha.dd;
  *Flash_ptr++ = alarma[i].fecha.mes;  
 } 
 
 *Flash_ptr++ = hora_llenado.hr;
 *Flash_ptr++ = hora_llenado.min;
 
 *Flash_ptr++ = porcentaje_llenado;
 *Flash_ptr++ = porcentaje_riego;  
 
 for(i=0; i<6; i++){
  for(j=0; j<6; j++){
    *Flash_ptr++ = programa[i].ud_on[j];
  }
  for(j=0; j<6; j++){
    *Flash_ptr++ = programa[i].ud_off[j];
  }
  *Flash_ptr++ = programa[i].hora_on.hr;    
  *Flash_ptr++ = programa[i].hora_on.min;
  *Flash_ptr++ = programa[i].hora_off.hr; 
  *Flash_ptr++ = programa[i].hora_off.min;
 }  
 *Flash_ptr++ = date.hora.seg;         
 *Flash_ptr++ = date.hora.min;         
 *Flash_ptr++ = date.hora.hr;         
 *Flash_ptr++ = date.fecha.dd;         
 *Flash_ptr++ = date.fecha.mes;         
 *Flash_ptr++ = date.fecha.aa;          

 FCTL1 = FWKEY;                  // Clear WRT bit
 FCTL3 = FWKEY + LOCK;           // Reset LOCK bit 
 _EINT();
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Cada vez que se vaya la alimentación principal y secundaria(batería), al 
// aparecer de nuevo, se cargaran todos los datos guardados en Flash el memoria RAM.
// La primera vez que se ejecuta el programa, la Flash está vacía, por lo que no 
// deberá leer de Flash.
void Read_Flash(void)
{
 char *Flash_ptr;  // Flash pointer
 unsigned int i, j; 

 Flash_ptr = (char *) 0x1000; //Se posiciona el puntero en el inicio del Segmento B.

 if(*Flash_ptr != 0xFF){      //podrá leer si hay algo escrito.
   for (i=0; i<3; i++){
    conf[i].on = *Flash_ptr++;
    conf[i].io = *Flash_ptr++;
   }
   for (i=0; i<8; i++){
    in[i].libre = *Flash_ptr++;
    in[i].remoto = *Flash_ptr++;
    in[i].direccion = *Flash_ptr++;
    *Flash_ptr++;
   }
   for (i=0; i<8; i++){
    out[i].libre = *Flash_ptr++;
    out[i].remoto = *Flash_ptr++;
    out[i].direccion = *Flash_ptr++;
    *Flash_ptr++;
   }
   for (i=0; i<6; i++){
    zona[i].on = *Flash_ptr++;
    zona[i].desconex = *Flash_ptr++;
    zona[i].remoto = *Flash_ptr++;
    zona[i].sal = *Flash_ptr++;
    zona[i].direccion = *Flash_ptr++;   //ocupa 1 byte, pero dejo libre el otro byte 
    *Flash_ptr++;                       //para q empiece en direccion par.
    zona[i].hora_off.hr = *Flash_ptr++;
    zona[i].hora_off.min = *Flash_ptr++;  
   } 
   for (i=0; i<4; i++){
    alarma[i].on = *Flash_ptr++;
    alarma[i].num_alarmas = *Flash_ptr++;
    alarma[i].hora.hr = *Flash_ptr++;
    alarma[i].hora.min = *Flash_ptr++;
    alarma[i].fecha.dd = *Flash_ptr++;
    alarma[i].fecha.mes = *Flash_ptr++;  
   }  
   
   hora_llenado.hr = *Flash_ptr++;  
   hora_llenado.min = *Flash_ptr++;
   
   porcentaje_llenado = *Flash_ptr++;
   porcentaje_riego = *Flash_ptr++; 
    
   for(i=0; i<6; i++){
    for(j=0; j<6; j++){
      programa[i].ud_on[j] = *Flash_ptr++;
    }
    for(j=0; j<6; j++){
      programa[i].ud_off[j] = *Flash_ptr++;;
    }
    programa[i].hora_on.hr = *Flash_ptr++;;    
    programa[i].hora_on.min = *Flash_ptr++;;
    programa[i].hora_off.hr = *Flash_ptr++;; 
    programa[i].hora_off.min = *Flash_ptr++;;
   } 
   date.hora.seg = *Flash_ptr++;         
   date.hora.min = *Flash_ptr++;            
   date.hora.hr = *Flash_ptr++;            
   date.fecha.dd = *Flash_ptr++;            
   date.fecha.mes = *Flash_ptr++;            
   date.fecha.aa = *Flash_ptr++; 
 }    
}

//***********************************************************************************
//ENTRADAS: maximo(valor máximo hasta el que puede introducir el usuario),
//          longitud(tamaño del valor máximo),
//          posicion(situación del cursor para escribir el número).
//SALIDAS: -
//FUNCIÓN: Se encarga de registrar el número introducido por el ususario, siempre que
// no se pase del valor máximo. Transformamos el número máximo y el número que vamos
// insertando, en cifras. Usa la función Entero y Pinta_num.
unsigned long Numero(unsigned long maximo, unsigned int longitud, unsigned char posicion)
{
 unsigned long max[7] = {0, 0, 0, 0, 0, 0, 0};
 unsigned long cifra[7] = {0, 0, 0, 0, 0, 0, 0};
 unsigned long final = 0; 
 unsigned char pos = posicion;
 unsigned int i = 0;
 
 final = maximo;
 
 cifra[0] = final/1000000;
 cifra[1] = (final%1000000)/100000;
 cifra[2] = (final%100000)/10000; 
 cifra[3] = ((final%100000)%10000)/1000; 
 cifra[4] = (((final%100000)%10000)%1000)/100;
 cifra[5] = ((((final%100000)%10000)%1000)%100)/10;
 cifra[6] = ((((final%100000)%10000)%1000)%100)%10; 
 
 max[0] = maximo/1000000;
 max[1] = (maximo%1000000)/100000;
 max[2] = (maximo%100000)/10000; 
 max[3] = ((maximo%100000)%10000)/1000; 
 max[4] = (((maximo%100000)%10000)%1000)/100;
 max[5] = ((((maximo%100000)%10000)%1000)%100)/10;
 max[6] = ((((maximo%100000)%10000)%1000)%100)%10; 
  
 i = 7 - longitud; //apunta a la primera cifra
 do{
  if (ON != 9){
   while (flanco == 0){}
   switch(ON){
    case 12: break;
    case 3 : ON = 9;
             modo = 0;
             final = Entero(cifra);
             if ((final == 0)|(cifra[i]==0)&(cifra[7-longitud]==max[7-longitud])){ 
                cifra[i] = max[i];                
             }else if (cifra[i]!= 0){
                cifra[i]--;                     
             } 
             break;
    case 13: ON = 9;
             modo = 0;
             cifra[i]++; 
             final = Entero(cifra);              
             if ((final>maximo)|(cifra[i] == 10)){  
                cifra[i] = 0;               
             }             
             break;
    case 22: if(modo == 1){
              ON = 9;
              if(pos == posicion + longitud-1){  
                pos = posicion;
                i = 7 - longitud;
              }else {
                pos++;
                i++;
              }
            }
            break;
   }
   final = Entero(cifra);

   Put_Ins(posicion);
   Pinta_num(cifra, longitud); 
   Put_Ins(pos);
  } 
 }while((ON!=12)&(modo!=2)); 
 while(flanco == 0){}

 return(final);
} 

//***********************************************************************************
//ENTRADAS: hora1(registro tipo hora),
//          hora2(registro tipo hora).
//SALIDAS: booleana
//FUNCIÓN: Compara dos registros tipo hora, sacando a la salida un '0' si no son 
// iguales, y un '1' si lo son.
bool Iguales(struct hora hora1, struct hora hora2)
{
 return((hora1.hr == hora2.hr) & (hora1.min == hora2.min) & (hora1.seg == hora2.seg));
} 

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: 
int Dia_Semana(void)
{
 int century, yr, dw;
 struct fecha f;
 
 f = date.fecha;

 if (f.mes < 3){
   f.mes = f.mes + 10;
   f.aa--;
 }else{
     f.mes = f.mes - 2;
 }   
 century = f.aa / 100;
 yr = f.aa % 100;
 dw = (((26 * f.mes - 2) / 10) + f.dd + yr + (yr / 4) +
   (century / 4) - (2 * century)) % 7;
 if (dw < 0){  
   return(5);
 }else if(dw==0){
   return(6);
 }else{ 
   return(dw - 1); 
 }   
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Función llamada por la interrupción de uno de los tres pines del puerto 1
// que se encarga de descifrar qué botón de los 11 posibles se ha pulsado.
void test_boton(void)
{
 unsigned int columna;
 
 P1DIR = 0x80;      //polling de entrada.
 P1OUT = 0;
 
 for(columna = 0; columna<4; columna++){
  if(!bitest(P1IN, BIT6)){          //fila 0
   ON = 0 + columna;
  }else if (!bitest(P1IN, BIT5)){   //fila 1
   ON = 10 + columna;   
  }else if (!bitest(P1IN, BIT4)){   //fila 2
   ON = 20 + columna;   
  }  
  if (columna == 0){
    P1DIR = 0x04;
  }else{
    P1DIR = P1DIR>>1;   //se desplaza el 1 una posición a la izq.
  }  
  P1OUT = 0;
 }
 P1DIR = 0x87;         //7, 2, 1 y 0 de salida
 P1OUT = 0;
  
 switch(ON){  //XOR leds.
  case 2 : P6OUT ^= 0x80; break;
  case 0 : P6OUT ^= 0x20; break;
  case 10: P6OUT ^= 0x08; break;
  case 20: P6OUT ^= 0x02; break;
  case 1 : P6OUT ^= 0x10; break;
  case 11: P6OUT ^= 0x04; break;
  case 21: P6OUT ^= 0x01; break;
 } 
}