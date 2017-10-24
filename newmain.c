/*
 * File:   newmain.c
 * Author: Aotian
 *
 * Created on October 23, 2017, 1:14 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
// PIC18F25K22 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config FOSC = INTIO67   // Oscillator Selection bits (Internal oscillator block)
#pragma config PLLCFG = OFF     // 4X PLL Enable (Oscillator used directly)
#pragma config PRICLKEN = ON    // Primary clock enable bit (Primary clock enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (Power up timer disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 190       // Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF       // Watchdog Timer Enable bits (WDT is always enabled. SWDTEN bit has no effect)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC1  // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<5:0> pins are configured as analog input channels on Reset)
#pragma config CCP3MX = PORTB5  // P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config T3CMX = PORTC0   // Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config P2BMX = PORTB5   // ECCP2 B output mux bit (P2B is on RB5)
#pragma config MCLRE = EXTMCLR  // MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled if MCLRE is also 1)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>
#define _XTAL_FREQ 20000000

//example of working with digital IO pins
void DigitalIO(){
    ANSELAbits.ANSA0 = 0;  //sets RA0 as digital pin(1 for analog)
    TRISAbits.RA0 = 0;  //sets RA0 as output
    PORTAbits.RA0 = 0;  //sets RA0 low
    ANSELA = 0;  //sets all RA pins as digital
    TRISA = 0; //seta all RA pins as output
    PORTA = 0; //sets all RA pins low
}
// must set ANSEL and TRIS properly manually outside of analog init
void analogInit(){
    ADCON1bits.PVCFG= 0;  //sets AREF high as VDD
    ADCON1bits.NVCFG= 0;  //sets AREF low as VSS
    ADCON2bits.ADFM=1;    //sets digital conversion as right justified  
}
//channel is number of analog pin, i.e. AN1 has channel number 1
int analogRead(int channel){
    ADCON0bits.CHS=channel;
    ADCON0bits.ADON = 1;
    while(PIR1bits.ADIF !=1){}
    int value = (int)(ADRESH)*256 + (int)(ADRESL);
    PIR1bits.ADIF =0;
    ADCON0bits.ADON = 0;
    return value;
}
void main(void) {
    printf("test/n");
    TRISC = 0;
    TRISA = 0;
    TRISB = 0;
    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSA0 = 1;
    ADCON0bits.CHS=0;
    ADCON1bits.PVCFG= 0;
    ADCON1bits.NVCFG= 0;
    ADCON2bits.ADFM=1;
    ADCON0bits.ADON = 1;
    float delay = 1;
    unsigned int val = 1;
    //ANSELC = 0;
    //ANSELCbits.ANSC4 = 1;
    while(1){
        if(PIR1bits.ADIF == 1){
            delay = ((int)(ADRESH)*256 + (int)(ADRESL))/1023.0;
            PIR1bits.ADIF = 0;
        }
        else{
            if(ADCON0bits.GO_DONE == 0){
                ADCON0bits.GO_DONE = 1;
            }
        }
        val = (unsigned int)(10000*delay);
        PORTC = 0xFF;
        PORTA = 0xFF;
        PORTB = 0xFF;
        for(int i =0; i<val; i++){
            __delay_us(1); 
        }
        PORTC = 0x00;
        PORTA = 0x00;
        PORTB = 0x00;
        for(int i =0; i<val; i++){
            __delay_us(1); 
        }
    }
    return;
}
