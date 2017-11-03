/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#define _XTAL_FREQ 8000000

// CONFIG1H
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON        // Internal/External Oscillator Switchover bit (Oscillator Switchover mode enabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#endif
#include <stdlib.h>
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/



void putch(char c) 
{
    // wait the end of transmission
    while (TXSTA1bits.TRMT == 0) {};
    TXREG1 = c; // send the new byte
}

void remote(){

    TRISC = 0xFF;
    TRISCbits.RC3 = 1;
    TRISCbits.RC4 = 1;


    
    
    TRISC |= 0x07;


    int voltage = 0;
    uint8_t i = 0;
    uint8_t i2= 0;
    int diff = 0;
    uint8_t porta = 0;
    uint8_t portc = 0;
    float freq = 0;
 

    return;
}

void I2CSlave(void) {


    TRISCbits.RC3 = 1;
    TRISCbits.RC4 = 1;

    //disable analog ports sharing pins with the LCD

    

    
  
    SSPADD =0x70;
    //TRISCbits.RC4 = 0;
    SSPCON1bits.SSPEN = 1;
    SSPCON1bits.SSPM = 0x6;
    SSPCON2bits.SEN = 0;
   // SSP1CON2bits.ACKDT =0; 
    
    
    //WE REMOVED THESE LINES FOR THE 4520!
    ////SSPCON2bits.AHEN = 0;
    ////SSPCON2bits.DHEN = 0;
    char buf = ' ';
    while(SSPSTATbits.S == 0){
        
    }
    while(PIR1bits.SSPIF == 0){
        
    }
    PIR1bits.SSPIF = 0;
    buf = SSPBUF;
    SSPSTATbits.BF = 0;
    
     while(PIR1bits.SSPIF == 0){
        
    }
    
    PIR1bits.SSPIF = 0;
    buf = SSPBUF;
    SSPSTATbits.BF = 0;
    if(buf == 0xAA)
    {
        PORTDbits.RD2 = 1;
    }
    return;
}

void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();
    
    //ANSELDbits.ANSD2 = 1; 
    
    TRISDbits.RD2 = 0;  //sets RA0 as output
    PORTDbits.RD2 = 0;  //sets RA0 low

PORTDbits.RD2 = 0;
    I2CSlave();

    while(1)
    {
        
    }
    /* TODO <INSERT USER APPLICATION CODE HERE> */

    while(1)
    {
        //putch(0xf0);
        //ADCON0bits
        
        PORTDbits.RD2 = 1;
        //PORTD |= (1<<2);
        __delay_ms(100);
        PORTDbits.RD2 = 0;
        //PORTD &= ~(1<<2);
        __delay_ms(100);
    }

}

