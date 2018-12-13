#include "config.h"

void configPIC(void)
{
    TRISA = OUTPUT;
    TRISB = OUTPUT;
    TRISC = OUTPUT;
    TRISD = OUTPUT;
    TRISE = OUTPUT;
    
    LED_TRIS = OUTPUT;
    
    ADCON1=0xFF;
    
    GIE = _ON;
    TMR1IE=1;
    PEIE=1;
    TMR1IP=1;

    
    TMR0IE=0;
    
    /*IPEN=1; //Enable Priority Interrupts
    PEIE=1; //Enable low priority peripheral  interrupts
    //Player 1
    INT0IE = 1; //Enable external interrupt
    INTEDG0 = 1; //Interrupt on rising edge of int pin
    //Player 2
    INT1IE = 1; //Enable external interrupt
    INTEDG1 = 1; //Interrupt on rising edge of int pin
    INT1IP=1; //high priority
    //Velocity
    INT2IE = 1; //Enable external interrupt
    INTEDG2 = 1; //Interrupt on rising edge of int pin
    INT2IP=0; //low priority
    //TMR1
    TMR1IE=1;
    TMR1IP=1;*/
    
    
    //T0CON = ObO1000110;
    
    //configurar pines
    
    /*TRISBbits.RB0=1;
    TRISBbits.RB1=1;
    TRISAbits.RA0=0;
    TRISAbits.RA1=0;
    TRISAbits.RA2=0;
    TRISEbits.RE0=0;
    TRISEbits.RE1=0;
    TRISEbits.RE2=0;
    TRISCbits.RC0=0;
    TRISCbits.RC1=0;
    LED1=_OFF;
    LED2=_OFF;
    LED3=_OFF;
    LED4=_OFF;
    LED5=_OFF;
    LED6=_OFF;
    LED7=_OFF;*/
    
}
