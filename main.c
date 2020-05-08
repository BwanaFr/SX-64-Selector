/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include "peripherials.h"
#include "interrupts.h"
#include "serial.h"
#include <xc.h>         /* XC8 General Include File */
#include <stdio.h>

#define CFG_ADDR 0
#define RESET_TIME 200
//#define SERIAL_DEBUG

void resetDrive(void);
void swapDisk(void);
void warmRestart(void);
void coldRestart(void);
void diskAddrSwap(void);
void kernalSwap(void);
void applyConfiguration(void);

typedef void (*Operation)(void);

struct Action {
    ms_t time;
    Operation opt;
    bool beep;
};
struct Action actions[] = {
    {50, NULL, false}, /* Skip button glitches */
    {200, resetDrive, false},
    {500, swapDisk, true},
    {1000, warmRestart, true},
    {1500, coldRestart, true},
    {2000, diskAddrSwap, true},
    {2500, kernalSwap, true}
};
union Config{
    struct {
        unsigned drvAddr8 : 1;
        unsigned kernal1 : 1;
    }cfg;
    uint8_t val;
};

static uint8_t nbActions = sizeof(actions)/sizeof(struct Action);
static union Config cfg;

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void main(void)
{
    initPeripherials();
#ifdef SERIAL_DEBUG
    initSerial();
#endif
    cfg.val = eeprom_read(0);
    applyConfiguration();
#ifdef SERIAL_DEBUG
    printf("We handle %u actions\n", nbActions);
    printf("Actual configuration:\n");
    printf("Internal drive as 8 : %s\n", cfg.cfg.drvAddr8 ? "yes" : "no");
    printf("Second kernal selected : %s\n", cfg.cfg.kernal1 ? "yes" : "no");
#endif  
    ms_t lastBtnEdge = 0;
    bool btnActive = false;
    uint8_t selAction = 0;
    while(1)
    {  
        if(!BUTTON){
            //Button is pressed
            if(!btnActive){
                btnActive = true;
                lastBtnEdge = millis();
            }
            ms_t duration = millis()-lastBtnEdge;
            uint8_t i;
            for(i=1;i<nbActions;++i) {
                if(duration<actions[i].time){
                    break;
                }
            }
            if((i-1) != selAction) {
                selAction = i-1;
#ifdef SERIAL_DEBUG
                printf("%u->%u\n", (int)selAction, (int)duration);
#endif
            }
            
            if(actions[selAction].beep && 
                ((duration - actions[selAction].time) < 100)){
                BUZZER = 0;
            }else{
                BUZZER = 1;
            }
        }else{
            BUZZER = 1;
            if(btnActive){
                btnActive = false;
                if((selAction != 0) && (actions[selAction].opt)){
                    PORTB = 0x1;
                    actions[selAction].opt();
                    PORTB = 0x0;
                }
                selAction = 0;
            }
        }
    }
}

void resetDrive(void)
{
#ifdef SERIAL_DEBUG
    printf("Reset drive\n");
#endif
    nDRVRESET = 0;
    __delay_ms(RESET_TIME);
    nDRVRESET = 1;
}

void swapDisk(void)
{
#ifdef SERIAL_DEBUG
    printf("Swap disk\n");
#endif
    //Put to low
    SDIECSWAP = 0;
    __delay_ms(300);
    //Put back to high impedance
    SDIECSWAP = 1;
}

void warmRestart(void)
{
#ifdef SERIAL_DEBUG
    printf("Warm restart\n");
#endif
    nRESET = 0;
    __delay_ms(RESET_TIME);
    nRESET = 1;
}

void coldRestart(void)
{
#ifdef SERIAL_DEBUG
    printf("Cold restart\n");
#endif
    nRESET = 0;
    nEXROM = 0;
    __delay_ms(RESET_TIME);
    nRESET = 1;
    __delay_ms(300);
    nEXROM = 1;
}

void diskAddrSwap(void)
{
#ifdef SERIAL_DEBUG
    printf("Disk addr swap\n");
#endif
    cfg.cfg.drvAddr8 = !cfg.cfg.drvAddr8;
    applyConfiguration();
    eeprom_write(CFG_ADDR, cfg.val);
    resetDrive();
}

void kernalSwap(void)
{
#ifdef SERIAL_DEBUG
    printf("Kernal swap\n");
#endif
    cfg.cfg.kernal1 = !cfg.cfg.kernal1;
    applyConfiguration();
    eeprom_write(CFG_ADDR, cfg.val);
    coldRestart();
}

void applyConfiguration(void)
{
    if(cfg.cfg.kernal1){
        KERNALSWAP = 0;
    }else{
        KERNALSWAP = 1;
    }
    if(cfg.cfg.drvAddr8){
        SDIECADDR = 0;  //SDIEC is now address 8 (high impedance)
        DRVADDR = 0;    //Internal drive is now address 9
    }else{
        SDIECADDR = 1;
        DRVADDR = 1;
    }
}