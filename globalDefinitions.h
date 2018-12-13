/* 
 * File:   globalDefinitions.h
 * Author: luisoliva
 *
 * Created on August 17, 2018, 4:19 PM
 */

#ifndef GLOBALDEFINITIONS_H
#define	GLOBALDEFINITIONS_H

#define _XTAL_FREQ 48000000


#define LED_TRIS TRISAbits.RA0
#define LED0 LATAbits.LA0
//#define LED_TRIS TRISAbits.RA0
#define UNOA LATAbits.LA1
#define DOSA LATAbits.LA2
#define LED3 LATEbits.LE0
#define LED4 LATEbits.LE1
#define LED5 LATEbits.LE2
#define LED6 LATCbits.LC0
#define LED7 LATCbits.LC1
#define TRESA LATBbits.LB4
#define CUATROA LATBbits.LB5

#define _ON 1
#define _OFF 0
#define OUTPUT 0
#define INPUT 1

/////LCD///////
#define COMMAND 0
#define DATA 1

#define ESCRIBIR 0
#define LEER 1

#define HABILITADO 1
#define DESHABILITADO 0

#define HIGH 1
#define LOW 0
//SENALES DE CONTROL
#define RS LATBbits.LB5 //
#define RW LATBbits.LB4 //
#define E  LATBbits.LB3 //
//PUERTOS DE CONTROL
#define D0 LATDbits.LD0
#define D1 LATDbits.LD1
#define D2 LATDbits.LD2
#define D3 LATDbits.LD3
#define D4 LATDbits.LD4
#define D5 LATDbits.LD5
#define D6 LATDbits.LD6
#define D7 LATDbits.LD7
#define BF LATDbits.LD7
//DISPLAY CURSOR PARPADEO   DS p.11
#define D LATDbits.LD2 //Display
#define C LATDbits.LD1 //Cursor
#define B LATDbits.LD0 //Blinking
//8-BIT 2-LINE 5X7
#define DL LATDbits.LD4  
#define N LATDbits.LD3
#define F LATDbits.LD2

#define eightbit_1line_5x7  0b00110000
#define eightbit_2lines_5x7 0b00111000
#define display_cursor_blink_OFF 0b00001000
#define display_cursor_blink_ON  0b00001111
#define CLEAR_DISPLAY 0b00000001
#define GO_HOME 0b00000010

#endif	/* GLOBALDEFINITIONS_H */

