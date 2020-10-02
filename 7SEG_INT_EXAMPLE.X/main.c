/*
 * File:   main.c
 * Author: Roland Pelayo
 *
 * Created on October 2, 2020, 5:40 PM
 */

#define _XTAL_FREQ 4000000
#include <xc.h>
#include <math.h>

//function prototypes
void enableExtInterrupt();
void enableTmrInterrupt(int prescale);
void setupPorts();
void incrementValues();
char numberToSegment(int number);

//global variables
int ones = 0;
int tens = 0;

//main routine
void main(void) {
    enableExtInterrupt();
    enableTmrInterrupt(6);
    setupPorts();
    while(1){
        
    }
    return;
}
//enable external interrupt routine
void enableExtInterrupt(){
    GIE = 1;
    INTE = 1;
}
//enable timer interrupt routine
void enableTmrInterrupt(int prescale){
    GIE = 1;
    T0IE = 1;
    int mask = 0x07 & prescale;
    OPTION_REG = OPTION_REG & mask;
}
//setup output and input pins routine
void setupPorts(){
    TRISB0 = 1;
    TRISC0 = 0;
    TRISC1 = 0;
    TRISD = 0;
}

//increment 7seg value routine
void __interrupt () ISR(){
    //check if both external interrupt enable and flag bits are set
    if(INTE && INTF){
        ones++;
        if(ones > 9){
            tens++;
            ones = 0;
        }
        if(tens > 9){
            tens = 0;
        }
        //clear interrupt flag
        INTF = 0;
    }
    if(T0IE && T0IF){
        if(RC0 == 1){
            RC0 = 0;
            RC1 = 1;
            PORTD = numberToSegment(tens);
        }else{
            RC0 = 1;
            RC1 = 0;
            PORTD = numberToSegment(ones);
        }
        //clear interrupt flag
        T0IF = 0;
    }

    return;
}
// number to seven segment binary routine
char numberToSegment(int number){
    switch(number){
        case 0:
          return 0b00111111;     //zero
          break;
        case 1:
          return 0b00000110;    //one
          break;
        case 2:
          return 0b01011011;     //two
          break;
        case 3:
          return 0b01001111;     //three
          break;
        case 4:
          return 0b01100110;     //four
          break;
        case 5:
          return 0b01101101;     //five
          break;
        case 6:
          return 0b01111101;     //six
          break;
        case 7:
          return 0b00000111;     //seven
          break;
        case 8:
          return 0b01111111;     //eight 
          break;
        case 9:
          return 0b01101111;     //nine  
          break;
        default:
          return 0b00111111;     //zero
          break;
    }
}