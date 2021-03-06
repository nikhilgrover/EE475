/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

void UART_Config();
inline void UART_WriteByte(char data);
inline char UART_BytesToRead();
signed int UART_ReadByte();

void UART_Config()
{
    //UART pin configuration
    TRISC |= 0xc0;
    ANSELCbits.ANSC6 = 0;
    ANSELCbits.ANSC7 = 0;
    
    PIE1bits.RC1IE = 1; //enable the UART receive interrupt

    TXSTA1bits.SYNC    =     0;   // ASYNCHRONOUS MODE     
    TX1STAbits.TX9 = 0;   // SELECT 8 BIT TRANSMISSION.   
    TXSTA1bits.TXEN    =     1;   // ENABLES TRANSMISSION.
    
    RC1STAbits.RX9 = 0;  
    RCSTA1bits.CREN    =     1;   // ENABLES CONTINUOUS RECEIVE.
    RCSTA1bits.SPEN = 1;//ENABLE SERIAL PORT
    
    //should get 9600 out of this @ xtal = 8MHz
    BAUDCON1bits.BRG16 =     1;  // ENABLE 16 BIT BAUD RATE REGISTER.
    TXSTA1bits.BRGH    =     1;  // SELECT HIGH SPEED BAUD RATE, FACTOR 4 AS THE DIVISOR IN THE FORMULA TO GENERATE THE BAUD RATE. 
    SPBRGH1            =   0x00;  // 16 BIT VALUE [SPBRGH1 : SPBRG1] 
    SPBRG1             =   0xcf;  //      
}

inline void UART_WriteByte(char data)
{
    while (TXSTA1bits.TRMT == 0) {};
        TXREG1 = data; // start sending the new byte
}

inline char UART_BytesToRead()
{
    return PIR1bits.RC1IF;
}

signed int UART_ReadByte()
{
    while (PIR1bits.RC1IF == 0) 
    { // wait for char11
        if (RCSTA1bits.OERR == 1) 
        {
            RCSTA1bits.OERR = 0; // clear overrun if it occurs
            RCSTA1bits.CREN = 0;
            RCSTA1bits.CREN = 1;
            return -1;
        }
    }
    char c = RCREG1;
    RCSTA1bits.OERR = 0; // clear overrun if it occurs
    RCSTA1bits.CREN = 0;
    RCSTA1bits.CREN = 1;
    return c;
}

#endif
