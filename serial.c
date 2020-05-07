/*
 * File:   serial.c
 * Author: mdonze
 *
 * Created on 13 November 2018, 14:55
 */

#include "serial.h"
#include <xc.h>

void initSerial(void)
{
   //Pin for UART
   TRISBbits.TRISB2 = 1;
   TRISBbits.TRISB1 = 1;
   //Clock divider
   SPBRG = DIVIDER;
   //Receive control register
   RCSTA = 0x0;
   //Serial port enabled
   RCSTAbits.SPEN = 1;
   //Continous receive enable
   RCSTAbits.CREN = 1;
   //TX enabled, high speed mode
   TXSTA = 0x24;
}

/**
 * Putch for printf support
 * @param byte
 */
void putch(char byte)
{
	while(!TXIF)
		continue;
	TXREG = byte;
}
