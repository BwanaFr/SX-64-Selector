/*
 * File:   interrupts.c
 * Author: mdonze
 *
 * Created on May 3, 2020, 6:44 PM
 */

#include <xc.h>
#include "interrupts.h"

static volatile ms_t millisValue=0;

// Timer 1 is configured with a 1:4 scaler
// Given the CLK freq of 19,600,000 Hz, Fosc/4 = 4,900,000Hz
//(Fosc/4)/Prescaler = 1,225,000 Hz
// 1E-3/((1/(Fosc/4)/Prescaler)-1 = 1224
// 0xFFFF - 1225 = 64331 -> 0xFB37
//Timer 1 high bits preset
#define TMR1_H_PRES 0xFB
//Timer 1 low bits preset
#define TMR1_L_PRES 0x37

void __interrupt () isr(void)
{
    if(TMR1IF && TMR1IE){
        TMR1H = TMR1_H_PRES;             // preset for timer1 MSB register
        TMR1L = TMR1_L_PRES;             // preset for timer1 LSB register        
        TMR1IF = 0;
        ++millisValue;
    }
}

ms_t millis(void)
{
    return millisValue;
}