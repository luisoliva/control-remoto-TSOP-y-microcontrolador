/*
 * File:   main_TurnOnLED.c
 * Author: luisoliva
 *
 * Created on August 17, 2018, 4:06 PM
 */
// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
     // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HSPLL_HS  // Oscillator Selection bits (HS oscillator, PLL enabled (HSPLL))
#pragma config FCMEN = OFF 
#pragma config BORV = 3// Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

// CONFIG3H
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

// CONFIG5L

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)


#include "config.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <xc.h>			// Encabezado para el compilador XC8

int count=0;
int tmr1Overflow=0;
double timerValue[32];
int index=-1;
int enableDOWN=1;
int enableUP=0;
double turnOnCode[32]={909,711,845,2371,852,708,845,708,852,708,852,708,852,708,852,708,845,2371,852,708,845,2371,845,2371,845,2371,845,2371,845,2371,845,2371};
double volumeUpCode[32]={908,2373,849,2373,849,2371,849,710,852,708,852,708,852,708,852,708,852,708,852,708,852,708,849,2373,849,2373,849,2373,849,2373,849,2373};
double volumeDownCode[32]={910,2360,850,2370,850,710,850,2365,852,708,850,710,850,710,850,710,850,710,850,710,850,2370,850,710,850,2370,850,2370,850,2370,850,2370};
double channelUpCode[32]={907,710,845,2371,850,710,845,710,845,2371,850,710,850,710,850,710,845,2371,850,710,845,2371,845,2371,850,710,845,2371,845,2371,845,2371};
double channelDownCode[32]={908,710,852,708,852,708,852,708,845,2371,852,708,852,708,852,708,845,2371,845,2371,845,2371,845,2371,852,708,845,2371,845,2371,845,2371};
//las señales estan empezando un bit antes de lo que yo quiero.
//las señales estan cortadas.


void TMR1Config(){
    T1CONbits.TMR1CS=0; //Every pulse by the internal osc will increase TMR1 register
    // w/48Mhz it'll increase every 0.0833us
    T1CONbits.RD16=1;
    T1CONbits.T1OSCEN=0;
    T1CONbits.T1CKPS0=1; //Psc,11-8;10-4;01-2;00-1
    T1CONbits.T1CKPS1=1;
    T1CONbits.T1RUN=0;
    
    TMR1ON=1;
    
}


void initgetUS(){
    TMR1=0;
    tmr1Overflow=0;
    
      
}

double getTimeUS(){
    return ((tmr1Overflow*65535)+TMR1);
}

void wait_in_ms(unsigned int time_in_ms){
    count=0;
    double ms=(time_in_ms*0.001);
    while(count < (375*ms)){
        while (!T0IF);
        T0IF = 0;
        count++;
        TMR0L = 6;
    }

}

void sumTmr1Overflow(){
    tmr1Overflow++;
}

void __interrupt() isrPlayer1(void){
    if (TMR1IF==1){
        sumTmr1Overflow();
        TMR1IF=0;
    }
}

int compareSignals(){
    int to=compareTurnOn();
    int vu=compareVolumeUp();
    int vd=compareVolumeDown();
    int cu=compareChannelUp();
    int cd=compareChannelDown();
    int values[5]={to,vu,vd,cu,cd};
    int signal=0;
    unsigned int numMenor=1000000;
    
    for(int ii=0;ii<5;ii++){
        if(values[ii]<numMenor){
            signal=ii+1;
            numMenor=values[ii];
        }
    }
    
    if (numMenor>200){
        signal=0;
    }
    
    return signal;
}

int compareTurnOn(){
    int acum=0;
    int sum[30];
    
    for (int i=0;i<30;i++){
        if (timerValue[i]>=turnOnCode[i]){
            sum[i]=timerValue[i]-turnOnCode[i];
        }else{
            sum[i]=turnOnCode[i]-timerValue[i];
        }
        
        acum=acum+sum[i];
    }
    
    return acum;
}

int compareVolumeUp(){
    int acum=0;
    int sum[32];
    
    for (int i=0;i<32;i++){
        if (timerValue[i]>=volumeUpCode[i]){
            sum[i]=timerValue[i]-volumeUpCode[i];
        }else{
            sum[i]=volumeUpCode[i]-timerValue[i];
        }
        
        acum=acum+sum[i];
    }
    
    return acum;
}

int compareVolumeDown(){
    int acum=0;
    int sum[32];
    
    for (int i=0;i<32;i++){
        if (timerValue[i]>=volumeDownCode[i]){
            sum[i]=timerValue[i]-volumeDownCode[i];
        }else{
            sum[i]=volumeDownCode[i]-timerValue[i];
        }
        
        acum=acum+sum[i];
    }
    
    return acum;
}

int compareChannelUp(){
    int acum=0;
    int sum[32];
    
    for (int i=0;i<32;i++){
        if (timerValue[i]>=channelUpCode[i]){
            sum[i]=timerValue[i]-channelUpCode[i];
        }else{
            sum[i]=channelUpCode[i]-timerValue[i];
        }
        
        acum=acum+sum[i];
    }
    
    return acum;
}

int compareChannelDown(){
    int acum=0;
    int sum[32];
    
    for (int i=0;i<32;i++){
        if (timerValue[i]>=channelDownCode[i]){
            sum[i]=timerValue[i]-channelDownCode[i];
        }else{
            sum[i]=channelDownCode[i]-timerValue[i];
        }
        
        acum=acum+sum[i];
    }
    
    return acum;
}


void Forward(void){    
    //MOTOR DERECHO
    UNOA=_OFF; //COMBINACION
    DOSA=_ON; //COMBINACION
    
    //MOTOR IZQUIERDO
    TRESA=_ON; //COMBINACION
    CUATROA=_OFF; //COMBINACION
}

void Left(void){
    //MOTOR DERECHO
    UNOA=_OFF; //COMBINACION
    DOSA=_ON; //COMBINACION
    
    //MOTOR IZQUIERDO
    TRESA=_ON; //COMBINACION
    CUATROA=_ON; //COMBINACION
}

void Right(void){
    //MOTOR DERECHO
    UNOA=_ON; //COMBINACION
    DOSA=_ON; //COMBINACION
    
    //MOTOR IZQUIERDO
    TRESA=_ON; //COMBINACION
    CUATROA=_OFF;
}
void Stop(void){
    //MOTOR DERECHO
    UNOA=_ON; //COMBINACION
    DOSA=_ON; //COMBINACION
    
    //MOTOR IZQUIERDO
    TRESA=_ON; //COMBINACION
    CUATROA=_ON; //COMBINACION
}
void Reverse(void){
    UNOA=_ON;
    DOSA=_OFF;
    
    TRESA=_OFF;
    CUATROA=_ON;
}

void main(void) {
    TMR1Config();
    configPIC();
    TRISCbits.RC2=1;

    
    while (1){
        
        //while(PORTCbits.RC2!=0);
        if (PORTCbits.RC2==0 && enableDOWN==1){
            index++;
            enableDOWN=0;
            enableUP=1;
            if(index==36){
                initgetUS();
                
            }
            
            if(index==38){
                timerValue[1]=getTimeUS();
                initgetUS();
               
            }
            
            if(index==40){
                timerValue[3]=getTimeUS();
                initgetUS();
                
            }
            
            if(index==42){
                timerValue[5]=getTimeUS();
                initgetUS();
             
            }
            
             if(index==44){
                timerValue[7]=getTimeUS();
                initgetUS();
            }
            
            if(index==46){
                timerValue[9]=getTimeUS();
                initgetUS();
            }
            
            if(index==48){
                timerValue[11]=getTimeUS();
                initgetUS();
            }
            
            if(index==50){
                timerValue[13]=getTimeUS();
                initgetUS();
            }
            
            if(index==52){
                timerValue[15]=getTimeUS();
                initgetUS();
            }
            
            if(index==54){
                timerValue[17]=getTimeUS();
                initgetUS();
            }
            
            if(index==56){
                timerValue[19]=getTimeUS();
                initgetUS();
            }
            
            if(index==58){
                timerValue[21]=getTimeUS();
                initgetUS();
            }
            
            if(index==60){
                timerValue[23]=getTimeUS();
                initgetUS();
            }
            
            if(index==62){
                timerValue[25]=getTimeUS();
                initgetUS();
            }
            
            if(index==64){
                timerValue[27]=getTimeUS();
                initgetUS();
            }
            
             if(index==66){
                timerValue[29]=getTimeUS();
                initgetUS();
            }
            
            if(index==68){
                timerValue[31]=getTimeUS();
                initgetUS();
                enableDOWN=0;
                enableUP=0;
            }
            
        }
        
        if (PORTCbits.RC2==1 && enableUP==1){
            index++;
            enableUP=0;
            enableDOWN=1;
            if (index==37){
                timerValue[0]=getTimeUS();
                initgetUS();
               
            }
            
            if (index==39){
                timerValue[2]=getTimeUS();
                initgetUS();
                
            }
            
            if (index==41){
                timerValue[4]=getTimeUS();
                initgetUS();
             
            }
            
            if (index==43){
                timerValue[6]=getTimeUS();
                initgetUS();
               
            }
            
            if (index==45){
                timerValue[8]=getTimeUS();
                initgetUS();
               
            }
            
            if (index==47){
                timerValue[10]=getTimeUS();
                initgetUS();
               
            }
            
            if (index==49){
                timerValue[12]=getTimeUS();
                initgetUS();
               
            }
            
            if (index==51){
                timerValue[14]=getTimeUS();
                initgetUS();
               
            }
            
            if (index==53){
                timerValue[16]=getTimeUS();
                initgetUS();
               
            }
            
            if (index==55){
                timerValue[18]=getTimeUS();
                initgetUS();
               
            }
            
            if (index==57){
                timerValue[20]=getTimeUS();
                initgetUS();
               
            }
            
            if (index==59){
                timerValue[22]=getTimeUS();
                initgetUS();
               
            }
            
            if (index==61){
                timerValue[24]=getTimeUS();
                initgetUS();
               
            }
            
            if (index==63){
                timerValue[26]=getTimeUS();
                initgetUS();
               
            }
            
             if (index==65){
                timerValue[28]=getTimeUS();
                initgetUS();
               
            }

            
            if (index==67){
                timerValue[30]=getTimeUS();
                initgetUS();
            }
            
        }
        
        if (enableDOWN==0 && enableUP==0){
            int comando=compareSignals();
            if (comando==1){
                Stop();
                //LED3=_ON;
                //__delay_ms(1000);
                //LED3=_OFF;
            }
        
            if (comando==2){
                Right();
                //LED4=_ON;
                //__delay_ms(1000);
                //LED4=_OFF;
            }
            
            if (comando==3){
                Left();
                //LED5=_ON;
                //__delay_ms(1000);
                //LED5=_OFF;
                
            }
            
            if (comando==4){
                Forward();
                //LED6=_ON;
                //__delay_ms(1000);
                //LED6=_OFF;
            }     
            
            if (comando==5){
                Reverse();
                //LED0=_ON;
                //__delay_ms(1000);
                //LED0=_OFF;
                
            }    
            
            enableDOWN=1;
            enableUP=0;
            //index=-1;
            RESET();
        } 
    }
}