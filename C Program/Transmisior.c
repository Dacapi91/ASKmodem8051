#include<8051.h>
#include<math.h>
#include<stdlib.h>


// Declaraci�n de las interrupciones
void Ts() interrupt 1;
void Cero() interrupt 0;       
void Uno() interrupt 2;        
void PuertoSerie() interrupt 4; 

//funci�n encargada de transmitir una trama
void Transmitir();             

char i=0;
char j=0;
char k=0;
char error=0; 
char comprobacion=0;   //byte donde se guarda la resta de comprobaci�n, �ltimo de cada trama
bit c=0;               //bit que indica el estado de la se�al '1' o '0'
bit e=0;               //bit de error
bit indicador=0;       //bit que se usa en el protocolo de parada y espera 

//Tabla que guarda los valores de un per�odo de la sinusoide portadora
const code char tabla[12]={192,239,255,239,192,128,64,17,0,17,64,128};  

//Cadena de caracteres para enviar
const code char Caracteres[13]={"HolaModemLoco"}; 

void main (void) 
{
    TMOD=0x22;  //modo 2 el timer 0 y el timer 1
    IE=0x97;    //habilitaci�n de las inerrupciones
    IP=0x15;    //Dando prioridad a las interrupciones
    SM0=0;
    SM1=1;      //configurando el modo 1 del puerto serie
    REN=1;      //habilitando recepci�n para poder recibir trama de confirmaci�n del receptor
    
    TH1=241;    //cargando la Vt de 2.08kbps
    
    IT0=1;      //interrucpciones externas activadas por flanco de ca�da
    IT1=1;
    
    TH0=-42;    //42us de Ts 
    TL0=-42;
    
    TR0=1;      //comienza Ts
    TR1=1;       
    TI=1;       //entra en la interrpuci�n del puerto serie para comenzar a transmitir 
    for(;;)
    {
        if(P0_0==0)
        {
           //TR0=!TR0;      
           //TR1=!TR1;      
           //TI=!TI;    
        }
    }    
     
          
}
    
//simula la PORTADORA a una frecuencia de 2kHz
void Ts (void) interrupt 1   
    {
       if(c==1)
       {
         P2=tabla[i]; //saca valores de la taqbla de la sinusoide por el puerto P2
       }
       else
       {
         P2=128;  //saca 0V a la salida del DAC
       }
       i++;
       if(i==12) //reiniciar la tabla de la sinusoide
       {
           i=0;
       }
      
    }
    
void Cero(void) interrupt 0 //flanco de bajada
{
    c=0;
    
    }
    
void Uno(void) interrupt 2  //flanco de subida
{
    c=1;
        
    }

void PuertoSerie(void) interrupt 4
{
        
    if(TI==1)//interrupci�n por transmisi�n
    {
      
       if(indicador==1)   //No se transmite hasta que llegue el byte de confirmaci�n
       {
           indicador=0;
          
       }
       else
       {
           Transmitir(); 
       }  
       TI=0;  
       return;
    }
       
    if(RI==1)//por recepci�n 
    {
        error=SBUF;
        if(error!=0x01)
        {
          j=j-6;       //dezplazo la longitud de la trama para retransmitir
          error=0;
        }
        
        Transmitir();  //transmite la pr�xima trama o retransmite la anterior
        RI=0;       
    }
}


void Transmitir(void)
{
  if(k!=6)
       {
         comprobacion=comprobacion-Caracteres[j];      //guarda la resta de comprobaci�n
         SBUF=Caracteres[j];                           //env�a los caracteres
         j++;
         k++;
        if(j==13)//para transmitir la cadena de caracteres constantemente
        {
            j=0;
            P1_7=!P1_7; //led verde indicador
        }
       
       }
    else
      {
        k=0;
        SBUF=comprobacion; //env�a el byte de comprobaci�n
        indicador=1;       //Se utiliza para indicar el fin de la trama y no transmitir hasta que llegue la confirmaci�n
        comprobacion=0;
        P1_6=!P1_6;       //indica que se ha transmitido una trama
        
      }  
}

