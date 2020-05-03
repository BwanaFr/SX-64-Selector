/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#define _XTAL_FREQ 4000000
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>         /* XC8 General Include File */


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void main(void)
{
  TRISB0 = 0; //RB0 as Output PIN
  while(1)
  {
    RB0 = 1;  // LED ON
    __delay_ms(1000); // 1 Second Delay
    RB0 = 0;  // LED OFF
    __delay_ms(1000); // 1 Second Delay
  }
}

