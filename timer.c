/*
 * File:   timer.c
 * Authors:
 *
 * Created on December 30, 2014, 8:07 PM
 */

#include <xc.h>
#include "timer.h"

//Uses timer 2
void delayUs(unsigned int delay) {
    
    TMR2 = 0; //clear timer 2
    PR2 = delay * 10-1;
    IFS0bits.T2IF = 0; //put flag down
    T2CONbits.ON = 1; //turn on timer 2
    while (IFS0bits.T2IF == 0) ;
    T2CONbits.ON = 0; //stop timer 2
}

void initTimer2() {
    //TODO: Initialize Timer 2.
    TMR2 = 0; //clear timer 2
    T2CONbits.TCKPS = 3; //initial prescalar to 256
    T2CONbits.TCS = 0; //setting the oscillator
    IFS0bits.T2IF = 0; //put flag down

}

void initTimer1(){
    TMR1=0;// clear timer1
    PR1=99999;//10MS
    T1CONbits.ON=1;// enable timer1
    T1CONbits.TCKPS=0;// pre-scalar is equal to 1
    T1CONbits.TCS=0;// internal oscillator 
    IFS0bits.T1IF=0;// Put down flag
    IPC1bits.T1IP=7;// configure the interrupt priority 
    IEC0bits.T1IE=1; // Enable the interrupt
}
