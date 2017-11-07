/*
 * File:   newmain.c
 * Author: Schuyler Horky
 *
 * Created on October 23, 2017, 1:14 PM
 */

//#define TEST_UART

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#define _XTAL_FREQ 80000000
// PIC18F25K22 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config FOSC = XT   // Oscillator Selection bits (Internal oscillator block)
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

#include <stdint.h>
#include "UART.h"


void I2Cinit()
{
    SSP1ADD =0x0F;
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC4 = 0;
    TRISCbits.RC3 = 1;
    TRISCbits.RC4 = 1;
    ANSELCbits.ANSC4 = 0;
    //TRISCbits.RC4 = 0;
    SSP1CON1bits.SSPEN = 1;
    SSP1CON1bits.SSPM = 0x8;
    SSP1CON2bits.RCEN = 0;
    //SSP1CON2bits.SEN = 1;
}

signed int readI2C(int addr)
{
    SSP1CON2bits.SEN = 1;
    int fail = 0;
    int buf = 0;
    while(PIR1bits.SSPIF == 0){
    }
    PIR1bits.SSPIF = 0;
    SSP1BUF = addr|0x01;
    while(PIR1bits.SSPIF == 0){
        
    }
    PIR1bits.SSP1IF = 0;
    SSP1CON2bits.RCEN = 1;
    while(PIR1bits.SSPIF == 0){
        
    }
    if(SSP1CON2bits.ACKSTAT == 0)
    {
        fail = 0;
    }
    
    buf = SSP1BUF;
    while(SSP1STATbits.BF == 1){
        
    }
    SSP1CON2bits.ACKDT = 1;
    SSP1CON2bits.ACKEN = 1;
    PIR1bits.SSPIF = 0;
    SSP1CON2bits.PEN = 1;
    while(PIR1bits.SSPIF == 0){}
    
    //clears interrupt flag
    PIR1bits.SSP1IF = 0;
    
    SSP1CON1bits.SSPEN = 0;
    SSP1CON1bits.SSPEN = 1;
    __delay_us(50);
    if(fail)
        return -1;
    return buf;
}

uint8_t writeI2C(int addr, char data)
{
    //start enable
    SSP1CON2bits.SEN = 1;
    uint8_t fail = 1;
    
    //start enable properly. Waits for enable
    while(PIR1bits.SSPIF == 0){ }
    
    //clears interrupt flag
    PIR1bits.SSPIF = 0;
    
    //sets first byte (address))
    SSP1BUF = addr;
    //waits for first byte transmission
    while(PIR1bits.SSPIF == 0){ }
    
    //clears interrupt flag
    PIR1bits.SSP1IF = 0;
    __delay_us(1);
    SSP1BUF = data;
    
    while(PIR1bits.SSPIF == 0){}
    
    if(SSP1CON2bits.ACKSTAT ==0)
    {
        fail = 0;
    }
  
    PIR1bits.SSPIF = 0;
    SSP1CON2bits.PEN = 1;
    
    while(PIR1bits.SSPIF == 0){}
    
    //clears interrupt flag
    PIR1bits.SSP1IF = 0;
    return fail;
}

struct packet
{
    uint32_t Temperature;
    uint32_t CO2;
    uint32_t Salinity;
    uint32_t FlowRate;
};

void SendPacket(struct packet* data)
{   
    char* buffer = data;
    for (int i = 0; i < sizeof(struct packet); i++)
		UART_WriteByte(buffer[i]);
}

enum State
{
    idle=1,
    initScan,
    termScan,
    read1x,
    read16x
};

char sensorBaseAddr = 0x70;
char sensorAddressNibble;
char cmdIDNibble;

enum State CurrentState;
enum State NextState;
struct packet testPacket;

void StartScan(char addr)
{   
   
    //I2C 
    //send start scan command and wait for ACK
    while(writeI2C(addr,0x03))
    {
        __delay_ms(10);
    }
    NextState = idle;
}

void EndScan(char addr)
{
    PORTB |= 1;
    //send end scan command and wait for ACK
    while(writeI2C(addr,0x04))
    {
        __delay_ms(10);    
    }
    
    PORTB &= ~1;
    
    NextState = idle;
}

void ConvertRawToMeas(struct packet* msg, char data[], int index)
{
    int32_t Temp = data[index] | (data[index+1]<<8);
    int32_t CO2 = data[index+2] | (data[index+3]<<8);
    int32_t Salinity = data[index+4] | (data[index+5]<<8);   
    int32_t FlowRate = data[index+6] | (data[index+7]<<8);
    
    msg->Temperature = ((Temp/1023.0*400-15)/100*27.75+4)*100.0;
    msg->CO2 = (CO2/1023.0*400*1.388+3.06)*100.0;
    msg->Salinity = (Salinity/1023.0*400*0.225-17.5)*100.0;
    msg->FlowRate = (FlowRate/228.0*10)*100*100.0;
}

void Read1x(char addr)
{
    PORTB |= 1;
    #ifdef TEST_UART
    struct packet p1;
    p1.Temperature = 101;
    p1.CO2 = 201;
    p1.Salinity = 301;
    p1.FlowRate = 401;

    SendPacket(&p1);
 
    NextState = idle;
    return;
    #endif
    //I2C command to 
    //request data from sensor (addr) for 1 reading
   
    char buffer[8];

    //send Readx1 command and wait for ack
    while(writeI2C(addr,0x01))
    {
        __delay_ms(10);
    }
    __delay_ms(10);
    while(writeI2C(addr,0x0a))
    {
        __delay_ms(10);
    }
    
    __delay_ms(300);
    for(int i=0;i<8;i++)
    {
        signed int data;
        data = readI2C(addr);//read data from remote sys    
        buffer[i] = (char)data;
    } 
    PORTB &= ~1;
    struct packet p;
    ConvertRawToMeas(&p, buffer,0);
    //p.Temperature = 100;
    //p.CO2 = 200;
    //p.Salinity = 300;
    //p.FlowRate = 400;
    //send reading to UART
    SendPacket(&p);
    NextState = idle;

}

void Read16x(char addr)
{
    //I2C command(s) to request data from sensor (addr) for 16 readings
    char buffer[128];


    //send Readx16 command and wait for ack
    while(writeI2C(addr,0x02))
    {
        __delay_ms(10);
    }
    
   
    __delay_ms(500);
    int index = 0;
    
    for(int i=0;i<16;i++)
    {
        PORTB |= 1;
        while(writeI2C(addr,0x0a))
        {
            __delay_ms(10);
        }
        
        PORTB &= ~1;
        //__delay_ms(10);
        //readI2C(addr);
        //__delay_ms(10);
        for(int j=0;j<8;j++)
        {
            __delay_ms(10);
            buffer[index] = readI2C(addr);
            index++;
        }
        struct packet p;
        ConvertRawToMeas(&p, buffer,i*8);
        
        SendPacket(&p);
    }
    
    
    //for(int i=0;i<8;i++)
    //{
    //    signed int data;
    //    data = readI2C(addr);//read data from remote sys    
    //    buffer[i] = (char)data;
    //    __delay_ms(500);
    //}


    //SendPacket(&p);
    //for(int i=0;i<16;i++)
    //{
    //   SendPacket(&p);
    //}
    NextState = idle;
}

void Idle()
{
    //if there's no bytes to read, don't read
    if(UART_BytesToRead()==0)
    {
        NextState = idle;
        #ifdef TEST_UART_BASIC
        PORTB |= 1;
        __delay_ms(5);
        PORTB &= ~1;
        __delay_ms(5);
        
        return;
        #endif
    }
    signed int cmd = UART_ReadByte(); 

    if(cmd==-1)//no command from PC
    {
        NextState = idle;
        return;
    }
    #ifdef TEST_UART_BASIC
    if(cmd == 65)
    {
        PORTB |= 1;
        __delay_ms(100);
        PORTB &= ~1;
        SendPacket(&testPacket);
        NextState = idle;
        return;
    }    
    NextState = idle;
    return;
    #endif

    sensorAddressNibble = (cmd & 0x0f)<<1;
    cmdIDNibble = cmd >> 4;
    
    switch(cmdIDNibble) 
    {
        case 0b0000://initiate scan
            NextState = initScan;//StartScan(sensorAddressNibble);
            break;
        case 0b0001://terminate scan
            NextState = termScan;
            break;
        case 0b0010://read 1x
            NextState = read1x;
            break;
        case 0b0011://read 16x
            NextState = read16x;
            break;
        case 0b0100://ping cmd. For debugging
            UART_WriteByte(0xa5);
            NextState = idle;
            break;
        default:
            NextState = idle;
            break;
    }   
}

void LocalSystemStateMachine()
{
    switch(CurrentState)
    {
        case idle:
            Idle();
            break;
        case initScan:
            StartScan(sensorBaseAddr | sensorAddressNibble);
            break;
        case termScan:
            EndScan(sensorBaseAddr | sensorAddressNibble);
            break;
        case read1x:
            Read1x(sensorBaseAddr | sensorAddressNibble);
            break;
        case read16x:
            Read16x(sensorBaseAddr | sensorAddressNibble);
            break;
    }

    CurrentState = NextState;
}

void main(void) 
{
    testPacket.Temperature = 0;
    testPacket.CO2 = 100;
    testPacket.Salinity = 200;
    testPacket.FlowRate = 300;
            
    TRISAbits.RA0 = 0;  //sets RA0 as output
    PORTAbits.RA0 = 0;  //sets RA0 low
    ANSELAbits.ANSA0 = 1;  //sets RA0 as analog input
    
    PORTBbits.RB0 = 0;
    ANSELBbits.ANSB0 = 0;
    TRISBbits.RB0 = 0;
   
    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSA0 = 1;
    ADCON0bits.CHS=0;
    ADCON1bits.PVCFG= 0;
    ADCON1bits.NVCFG= 0;
    ADCON2bits.ADFM=1;
    ADCON0bits.ADON = 1;

    PORTB &= ~1;
    I2Cinit();
    UART_Config();

    CurrentState = idle;

    while(1)
    {
        LocalSystemStateMachine();
    }
}

