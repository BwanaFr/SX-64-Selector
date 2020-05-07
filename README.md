# SX-64-Selector
A small MCU (PIC16F628) program for controlling a Commodore SX-64, in particular:
* SX-64 /RESET line (warm reset)
* SX-64 /EXTROM line (cold reset)
* SX-64 Internal drive /RESET line
* SX-64 internal drive address selection (8-9)
* SD2IEC address selection (8-9)
* SD2IEC disk swap button input
* Kernal selection betweeen original kernal and JiffyDOS

## Usage
The original SX-64 reset button will be connected to the MCU.
Depending on press duration on this button, some actions will be taken:
1. Less than 2 seconds
SD2IEC disk swap.
2. Between 2 and 4 seconds
SX-64 Warm reset
3. Between 4 and 6 seconds
SX-64 cold reset
4. Between 6 and 8 seconds
Swap addresses between SD2IEC and internal drive
5. More than 8 seconds
Swap between kernals and cold reset the computer
