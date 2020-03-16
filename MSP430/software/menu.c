#include "menu.h"
#include "functions.h"


bool flanco;                 // s
unsigned int modo=0;
unsigned int ON = 9;         //Botón pulsado.
unsigned int salidas_ocup[9]  = {1, 1, 1, 1, 1, 1, 1, 1, 1};  // Todas las salidas libres.
unsigned int entradas_ocup[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};  // Todas las entradas libres.

//***********************************************************************************
//ENTRADAS: linea
//SALIDAS: -
//FUNCIÓN: Pinta la línea que se corresponde con el número que le llega de entrada.
void Linea(unsigned int linea)
{
 Limpia_Linea(LINEA2);
 switch(linea){
  case 1:   Put_Str(cmanual); break;
  case 11:  Put_Str(zonas); break;
  case 12:  Put_Str(manualmotor); break;
  case 13:  Put_Str(manualestado); break;
//  case 2:   Put_Str(riego); break;
//  case 21:  Put_Str(hon); break;
//  case 22:  (tmpo_caudal)? Put_Str(litro) : Put_Str(hoff); 
//            break;
  case 3:   Put_Str(estado); break;
  case 4:   Put_Str(config); break;
  case 5:   Put_Str(conf_IO); break;
  case 51:  Put_Str(zonas); break;
  case 52:  Put_Str(salidas); break;
  case 53:  Put_Str(entradas); break;
  case 54:  Put_Str(ver); break;
  case 541: Put_Str(zonas); break;
  case 542: Put_Str(salidas); break;
  case 543: Put_Str(entradas); break;
  case 6:   Put_Str(conf_AL); break;
  case 7:   Put_Str(reloj); break;
  case 71:  Put_Str(hora); break;
  case 72:  Put_Str(fech); break;    
  case 8:   Put_Str(deposito); break;
  case 81:  Put_Str(llenado); break;
  case 82:  Put_Str(min_llenado); break;
  case 83:  Put_Str(en_riego); break;
  case 9:   Put_Str(guarda); break;  
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
  case 1:  Manual(); break;
  case 11: Unidad(); break;
  case 12: Motor(); break;
  case 13: Estado_Dep(); break;
 // case 2: Programacion(); break;
  case 3: Estado(); break;
  case 4: Conf(); break;
  case 5: Conf_IO(); break;
  case 51: Zonas(); break;
  case 52: IN_OUT(1, out, salidas_ocup); break;
  case 53: IN_OUT(0, input, entradas_ocup); break;
  case 54: Ver_Conf(); break;
  case 6: Conf_AL(); break;
  case 7: Reloj(); break;
  case 71: Poner_Hora(5, 0, 0); break;
  case 72: Poner_Fecha(); break;
  case 8:  Deposito(); break;
  case 81: Poner_Hora(1, 0, 0); break;
  case 82: Porcentaje(0); break;
  case 83: Porcentaje(1); break;
  case 9:  Guarda_Conf(); break;
 } 
 while(flanco == 0);       //se controla mejor la pulsación cuando queremos regresar.
 ON = 9;
 Put_Ins(Set_Display +Display_On + Cursor_Off + Blink_Off);
}

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

 do {
   if(ON != 9){
    while (flanco == 0);
    switch (ON)
    {
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
 } while(ON!=12);
 while(flanco == 0);
 ON = 9; 
 modo = 0;
}

