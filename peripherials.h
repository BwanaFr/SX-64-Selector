/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File: peripherials.h
 * Author: mdonze
 * Comments: I/O utilities
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PERI_INCLUDED_H
#define	PERI_INCLUDED_H

#define _XTAL_FREQ 4000000
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>       /* For true/false definition */
#include <stdint.h>

//Buzzer (PWM output)
#define BUZZER TRISBbits.TRISB3
//LED (for debug)
#define LED PORTBbits.RB0
//Button for interacting with this
#define BUTTON PORTAbits.RA0
// /RESET of SX64
#define nRESET TRISAbits.TRISA1
// /EXROM of SX64
#define nEXROM TRISAbits.TRISA2
// SD2IEC address selection
#define SDIECADDR TRISBbits.TRISB4
// Internal drive address selection
#define DRVADDR TRISBbits.TRISB5
// Internal drive reset
#define nDRVRESET TRISBbits.TRISB6
// SD2IEC disk swap
#define SDIECSWAP TRISBbits.TRISB7
//Kernal switch (between JiffyDOS and stock kernal)
#define KERNALSWAP TRISAbits.TRISA3


// Timer 1 is configured with a 1:1 scaler aiming a 1ms clock
// Given the CLK freq of 4,000,000 Hz, Fosc/4 = 1,000,000Hz
//(Fosc/4)/Prescaler = 1,000,000 Hz
// 1E-3/((1/(Fosc/4)/Prescaler)-1 = 999
// 0xFFFF - 1000 = 64535 -> 0xFC17
//Timer 1 high bits preset
#define TMR1_H_PRES 0xFC
//Timer 1 low bits preset
#define TMR1_L_PRES 0x17

/**
 * Initialize peripherials (I/O)
 */
void initPeripherials(void);


#endif	/* XC_HEADER_TEMPLATE_H */

