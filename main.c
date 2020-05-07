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
    {1200, warmRestart, true},
    {1700, coldRestart, true},
    {2200, diskAddrSwap, true},
    {2700, kernalSwap, true}
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
    initSerial();
    
    cfg.val = eeprom_read(0);
    applyConfiguration();
    
    printf("We handle %u actions\n", nbActions);
    printf("Actual configuration:\n");
    printf("Internal drive as 8 : %s\n", cfg.cfg.drvAddr8 ? "yes" : "no");
    printf("Second kernal selected : %s\n", cfg.cfg.kernal1 ? "yes" : "no");
    
    ms_t lastBtnEdge = 0;
    bool btnActive = false;
    uint8_t selAction = 0;
    while(1)
    {   
        if(!BUTTON){
            LED = 1;
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
                printf("%u->%u\n", (int)selAction, (int)duration);
            }
            
            if(actions[selAction].beep && 
                ((duration - actions[selAction].time) < 100)){
                BUZZER = 0;
            }else{
                BUZZER = 1;
            }
        }else{
            LED = 0;
            BUZZER = 1;
            if(btnActive){
                if(actions[selAction].opt)
                    actions[selAction].opt();
            }
            btnActive = false;
        }
    }
}

void resetDrive(void)
{
    printf("Reset drive\n");
    nDRVRESET = 0;
    __delay_ms(RESET_TIME);
    nDRVRESET = 1;
}

void swapDisk(void)
{
    printf("Swap disk\n");
    //Put to low
    SDIECSWAP = 0;
    __delay_ms(300);
    //Put back to high impedance
    SDIECSWAP = 1;
}

void warmRestart(void)
{
    printf("Warm restart\n");
    nRESET = 0;
    __delay_ms(RESET_TIME);
    nRESET = 1;
}

void coldRestart(void)
{
    printf("Cold restart\n");
    nRESET = 0;
    nEXROM = 0;
    __delay_ms(RESET_TIME);
    nRESET = 1;
    __delay_ms(300);
    nEXROM = 1;
}

void diskAddrSwap(void)
{
    printf("Disk addr swap\n");
    cfg.cfg.drvAddr8 = !cfg.cfg.drvAddr8;
    applyConfiguration();
    eeprom_write(CFG_ADDR, cfg.val);
    resetDrive();
}

void kernalSwap(void)
{
    printf("Kernal swap\n");
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