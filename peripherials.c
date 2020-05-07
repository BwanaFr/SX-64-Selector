/*
 * File:   peripherials.c
 * Author: mdonze
 *
 * Created on May 3, 2020, 6:48 PM
 */

#include <xc.h>
#include "peripherials.h"
#include "interrupts.h"

/**
 * Initialize peripherials (I/O)
 */
void initPeripherials(void)
{
    
    //No interrupts
    INTCON = 0x0;
    PIE1 = 0x0;
    
    //Configure timer 1 (millis counter)
    T1CONbits.T1CKPS1 = 0;   // bits 5-4  Prescaler Rate Select bits
    T1CONbits.T1CKPS0 = 0;   // bit 4
    T1CONbits.T1OSCEN = 0;   // bit 3 Timer1 Oscillator Enable Control bit 1 = on
    T1CONbits.nT1SYNC = 1;   // bit 2 Timer1 External Clock Input Synchronization Control bit...1 = Do not synchronize external clock input
    T1CONbits.TMR1CS = 0;    // bit 1 Timer1 Clock Source Select bit...0 = Internal clock (FOSC/4)
    T1CONbits.TMR1ON = 1;    // bit 0 enables timer
    TMR1H = TMR1_H_PRES;     // preset for timer1 MSB register
    TMR1L = TMR1_L_PRES;     // preset for timer1 LSB register
    
    //Enable interrupt on timer 1
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    //PWM for buzzer (2.2KHz)
    PR2 = 0b01110001;
    T2CON = 0b00000101;
    CCPR1L = 0b00111000;
    CCP1CON = 0b00111100;
    
    //All ports to 0
    PORTA = 0x0;
    PORTB = 0x0;
    
    //All inputs (no comparator)
    CMCON = 0x07;
    //All outputs except button (RA0)
    TRISA = 0xFF;
    
    //All port B are outputs but
    //force them to Z
    TRISB = 0xFF;
    
    //Except LED
    TRISBbits.TRISB0 = 0;
    
}
