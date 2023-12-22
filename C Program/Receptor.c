#include<8051.h>
#include<math.h>
#include<stdlib.h>


void PuertoSerie() interrupt 4; 
char Recibido[30];  //guarda las tramas recibidas
char i=0;
char k=0;           //byte de la trama
char suma=0;
const code char ByteError=0xFE;   //byte indicador del error
const code char ByteOk=0x01;      //byte de confirmaci�n

void main (void) 
    {
    TMOD=0x20;   //modo 2 el timer 1 
    IE=0x90;     //habilitaci�n de interrupciones
    IP=0x10;     //prioridad de las interrupciones 
    SM0=0;
    SM1=1;       //configurando el modo 1 del puerto serie
    
    TH1=241;     //cargando la Vt de 2.08kbps
        
    REN=1;       //habilitar la recepci�n
    TR1=1;       
          
    for(;;){
       
        
        } 
    }
    
    
void PuertoSerie(void) interrupt 4
{
    if(TI==1)      //no hacer nada cuando haya interrupci�n por transmisi�n
    {
        TI=0;
        return;
    }
   
    if(k!=6)
    {
        
        Recibido[i]=SBUF;      //guarda las tramas recibidas
        suma=suma+Recibido[i]; // realiza la suma de comprobaci�n
        i++;
        k++;
               
        if(i==30)             //buffer circular
        {
            i=0;
            P1_2=!P1_2;       //indica reinicio del buffer circular
        } 
    }
    else
    {
        k=0;
        Recibido[i]=SBUF;
        if((suma+Recibido[i])==0x00) //verifica la suma de comprobaci�n
        {
            P1_0=!P1_0;       //indica que la transmisi�n de la trama fue un �xito
            P1_1=1;           //indica que si hubo un error fue corregido
            SBUF=ByteOk;     //env�a byte de confirmaci�n al transmisor
            
        }
        else
        {
            P1_1=0;           //indica que hubo un error enciende led rojo
            SBUF=ByteError;   //env�a byte de error al transmisor
            i=i-6;            //para sobreescribir el error
        }
        suma=0;
       
    }
    
   RI=0; 
       
}