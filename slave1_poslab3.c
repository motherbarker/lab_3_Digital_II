/* 
 * File:   slave_poslab3
 * Device: PIC16F887
 * Author: Carlos Valdez
 *Compiler: XC8 (v2.41)
 * 
 * Program: 
 * Hardware:
 * 
 * Created: 
 * Last updated:
 */

/*--------------------------------- LIBRARIES --------------------------------*/
#include <xc.h>
#include "ADC.h"
#include "SPI.h"

/*---------------------------- CONFIGURATION BITS ----------------------------*/
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

/*----------------------- GLOBAL VARIABLES & CONSTANTS -----------------------*/
#define _XTAL_FREQ 8000000
/*-------------------------------- PROTOTYPES --------------------------------*/
void setup(void);
/*------------------------------- RESET VECTOR -------------------------------*/

/*----------------------------- INTERRUPT VECTOR -----------------------------*/
void __interrupt() isr(void){
    if(SSPIF){
        if(spiRead() == 'P');
            spiWrite(PORTB);
        SSPIF = 0;
    }
}

/*--------------------------- INTERRUPT SUBROUTINES --------------------------*/

/*---------------------------------- TABLES ----------------------------------*/

/*----------------------------------- MAIN -----------------------------------*/
int main(void) {
    setup();
    while(1){
        //Loop
        //Capture pot val
        __delay_ms(5);
        PORTB = adc_read()>>8;
    }
}
/*-------------------------------- SUBROUTINES -------------------------------*/
void setup(void){
    TRISA = 0b00100001;  //RA0 as input
    PORTA = 0;
    ANSEL = 1;  //AN0 - RA0 as analog
    ANSELH= 0;
    
    TRISB = 0;  //POT val
    PORTB = 0;
    
    //OSCILLATOR CONFIG
    OSCCONbits.IRCF = 0b111;  //Internal clock frequency 8MHz
    SCS = 1;
    
    //Initialize ADC. Left, Vdd/Vss, 8MHz, AN0.
    adc_init(0, 0, 8, 0);
    
    //SPI
    GIE = 1;
    PEIE = 1;
    SSPIE = 1;
    SSPIF = 0;
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);    
}