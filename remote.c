/*
 * File:   remote.c
 * Author: Aotian
 *
 * Created on November 5, 2017, 12:35 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h>
#include <stdint.h>
#include "LCD5110.h"
// PIC18F25K22 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config FOSC = INTIO67   // Oscillator Selection bits (Internal oscillator block)
//#pragma config FOSC = XT
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

int CO2 = 0;
int salinity = 0;
int flow = 0;
int temp = 0;

void analogInit(){
    ADCON1bits.PVCFG= 1;  //sets AREF high as VDD
    ADCON1bits.NVCFG= 0;  //sets AREF low as VSS
    ADCON2bits.ADFM=1;    //sets digital conversion as right justified  
}
//channel is number of analog pin, i.e. AN1 has channel number 1
int analogRead(int channel){
    ADCON0bits.CHS=channel;
    ADCON0bits.ADON = 1;
    ADCON0bits.GODONE = 1;
    while(PIR1bits.ADIF !=1){}
    int value = (int)(ADRESH)*256 + (int)(ADRESL);
    PIR1bits.ADIF =0;
    ADCON0bits.ADON = 0;
    return value;
}

void I2CSlaveInit(){
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC4 = 0;
    TRISCbits.RC3 = 1;
    TRISCbits.RC4 = 1;
    ANSELCbits.ANSC4 = 0;
    SSP1CON1bits.SSPEN = 1;
    SSP1CON1bits.SSPM = 0xE;
    SSP1CON2bits.SEN = 0;
   // SSP1CON2bits.ACKDT =0; 
    SSP1CON3bits.AHEN = 0;
    SSP1CON3bits.DHEN = 0;
    SSP1STATbits.SMP = 0;
    //SSP1CON1=0x36;
    //SSP1STAT = 0x00;
    //SSP1CON2 = 0x01;
    //PIR1bits.SSP1IF = 0;          // Clear MSSP interrupt request flag
    //PIE1bits.SSPIE = 1;             // Enable MSSP interrupt enable bit
    //INTCONbits.GIE_GIEH  = 1;       // GIE/GIEH: Global Interrupt Enable bit
    //INTCONbits.PEIE_GIEL = 1;       // PEIE/GIEL: Peripheral Interrupt Enable bit
}
void remoteInit(){
    ANSELC = 0x00;
    TRISC = 0xFF;
    TRISAbits.RA6 = 1;
    TRISAbits.RA7 = 1;
    ANSELBbits.ANSB1 = 0;
    TRISBbits.RB1 = 1;
    ANSELA &= 0x3F;
    ANSELCbits.ANSC2 = 0;
    ANSELCbits.ANSC5 = 0;
    ANSELCbits.ANSC6 = 0;
    ANSELCbits.ANSC7 = 0;
    ANSELBbits.ANSB0 = 0;
    ANSELBbits.ANSB0 = 0;
    ANSELBbits.ANSB1 = 0;
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;
    ANSELBbits.ANSB4 = 0;
    TRISA |= 0x07;
    ANSELA |=0x07;
    TRISAbits.RA0 = 1;
    ANSELAbits.ANSA0 = 1;
    TRISAbits.RA1 = 1;
    ANSELAbits.ANSA1 = 1;
    TRISAbits.RA5 = 1;
    ANSELAbits.ANSA5 = 1;
    analogInit();
    TRISCbits.RC7 = 0;
    TRISCbits.RC6 = 0;
    ANSELCbits.ANSC7 = 0;
    ANSELBbits.ANSB5 = 0;
    TRISBbits.RB5=0;
    ANSELAbits.ANSA2 = 0;
    TRISAbits.RA2 = 0;
    TRISAbits.RA4 = 0;
    ANSELC &=0xF8;
    ANSELCbits.ANSC5 = 0;
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC4 = 0;
    TRISCbits.RC3 = 1;
    TRISCbits.RC4 = 1;
    ANSELCbits.ANSC4 = 0;
    LCD_Config(&PORTB,0,1,2,3,4);
    LCD_Init();
    
    //erases all pixels from LCD
    LCD_ClearAll();
}
void scanSensors(){
    char buff[14];
    int length = 0;
    uint8_t i = 0;
    uint8_t i2= 0;
    int diff = 0;
    uint8_t porta = 0;
    float freq = 0;
        CO2 = analogRead(0);
        length = sprintf(buff, "CO2: %0.2f ppm", CO2/1023.0*400*1.388+3.06); //1023.0*400*1.388+3.06
        for(int j =length; j<14; j++){
            buff[j] = ' ';
        }
        LCD_Goto(1,0);
        LCD_WriteStr(buff, 14);
                
        salinity = analogRead(1);
        length = sprintf(buff, "Sal: %0.1f ppt", salinity/1023.0*400*0.225-17.5);
        for(int j =length; j<14; j++){
            buff[j] = ' ';
        }
        LCD_Goto(2,0);
        LCD_WriteStr(buff, 14);
        
        ANSELAbits.ANSA0 = 0;
        ANSELAbits.ANSA1 = 0;
        temp = analogRead(4);
        length = sprintf(buff, "Temp: %0.1f C", (temp/1023.0*400-15)/100*27.75+4);
        for(int j =length; j<14; j++){
            buff[j] = ' ';
        }
        LCD_Goto(3,0);
        LCD_WriteStr(buff, 14);
        ANSELAbits.ANSA0 = 1;
        ANSELAbits.ANSA1 = 1;
        
        length = sprintf(buff, "      %0.1f F", ((temp/1023.0*400-15)/100*27.75+4)*1.8+32);//(voltage/1023.0*400-15)/100*27.75*1.8+32
        for(int j =length; j<14; j++){
            buff[j] = ' ';
        }
        LCD_Goto(4,0);
        LCD_WriteStr(buff, 14);
        
        porta = 0;
        PORTCbits.RC6 = 0;
        PORTCbits.RC6 = 1;
        for( int k = 0; k<8; k++){
            porta += (((PORTB&0x02)<<6)>>k);
            PORTCbits.RC7 = 0;
            __delay_us(1);
            PORTCbits.RC7 = 1;
        }
        //i =((porta&0x80)>>7) + ((porta&0x40)>>5)+((portc&0x07)<<2)+(portc&0xE0);
        i=porta;
        __delay_us(1000);
        porta =0;
         PORTCbits.RC6 = 0;
        PORTCbits.RC6 = 1;
        for( int k = 0; k<8;k++){
            porta += (((PORTB&0x02)<<6)>>k);
            PORTCbits.RC7 = 0;
            __delay_us(1);
            PORTCbits.RC7 = 1;
        }
        //i2=((porta&0x80)>>7) + ((porta&0x40)>>5)+((portc&0x07)<<2)+(portc&0xE0);
        i2=porta;
        if(i2<i){
            flow = i2+255-i;
        }
        else{
            flow = i2-i;
        }
        freq = flow/228.0*10;
        length = sprintf(buff, "Flow: %0.2f L", freq);
        for(int j =length; j<14; j++){
            buff[j] = ' ';
        }
        LCD_Goto(5,0);
        LCD_WriteStr(buff, 14);


}

//sets pins connected to SRAM data as output
void out(){
    TRISAbits.RA6 = 0;
    TRISC &= 0xF8;
    TRISCbits.RC5 = 0;
}

//sets pins connected to SRAM data as input
void in(){
    TRISAbits.RA6 = 1;
    TRISC |= 0x07;
    TRISCbits.RC5 = 1;
}

//writes data to SRAM data lines
void setData(int data){
    PORTAbits.RA6 =(data)&1;
    PORTCbits.RC0 = (data>>1)&1;
    PORTCbits.RC1 = (data>>2)&1;
    PORTCbits.RC2 =(data>>3)&1;
    PORTCbits.RC5 =(data>>4)&1;
}

//reads data from SRAM data lines
int readData(){
    int data = 0;
    data+=PORTAbits.RA6;
    data+=PORTCbits.RC0*2;
    data+=PORTCbits.RC1*4;
    data+=PORTCbits.RC2*8;
    data+=PORTCbits.RC5*16;
    return data;
}

//sets SRAM address lines
void setAddr(int data){
    for(int i =0; i<7; i++){
        PORTAbits.RA2 = (data>>(6-i))&1;
        PORTCbits.RC7 = 0;
        PORTCbits.RC7 = 1;
    }
    PORTCbits.RC7 = 0;
    PORTCbits.RC7 = 1;
}

// enables writing to SRAM
void writeEnable(){
    PORTAbits.RA4 = 0;
    PORTBbits.RB5 = 1;
}

// enables reading from SRAM
void readEnable(){
    PORTAbits.RA4 = 1;
    PORTBbits.RB5 = 0;
}

// disables both read and write from SRAM
void disable(){
    PORTAbits.RA4 = 1;
    PORTBbits.RB5 = 1;
}

//writes data to addr of SRAM
void SRAMWrite(int addr, int data){
    setAddr(addr);
    writeEnable();
    setData(data);
    disable();
}

//writes all sensor data to SRAM
void allWrite(int addr){
    int current = addr;
    int data[8];
    data[0] = flow/32;
    data[1] = flow%32;
    data[2]=salinity/32;
    data[3]=salinity%32;
    data[4]=CO2/32;
    data[5]=CO2%32;
    data[6]=temp/32;
    data[7]=temp%32;
    for(int i =0; i<8; i++){
        SRAMWrite(current, data[i]);
        current++;
        if(current>=128){
            current-=128;
        }
    }
}
void sendI2C(int data){
    int buf = 0;
    PIR1bits.SSPIF = 0;
    buf = SSP1BUF;
    if(SSP1CON1bits.SSPOV == 1){
        SSP1CON1bits.SSPOV = 0;
        SSP1CON1bits.SSPEN = 0;
        SSP1CON1bits.SSPEN = 1;
    }
    while(SSP1STATbits.BF == 0){
    }
    while(PIR1bits.SSPIF = 0){
        
    }
    PIR1bits.SSPIF = 0;
    buf = SSP1BUF;
    __delay_us(1);
    SSP1BUF = data;
    __delay_us(1);
    SSP1CON1bits.CKP = 1;
     while(PIR1bits.SSPIF = 0){
        
    }
    PIR1bits.SSPIF = 0;
    while(SSP1STATbits.BF == 1){
        
    }
    SSP1CON1bits.SSPEN = 0;
    SSP1CON1bits.SSPEN = 1;
}
void main(void) {
    I2CSlaveInit();
    SSP1ADD =0x70;
    remoteInit();
    int scan = 0;
    int count = 0;
    int buf;
    int addr= 0;
    int send =0;
    int data[8];
    while(1){
        SSP1CON1bits.SSPEN = 0;
        SSP1CON1bits.SSPEN = 1;
        if(scan){
            scanSensors();
            allWrite(addr);
            addr+=8;
            if(addr>=128){
                addr-=128;
            }
            count =0;
            PIR1bits.SSPIF = 0;
            buf = SSP1BUF;
            if(SSP1CON1bits.SSPOV == 1){
                SSP1CON1bits.SSPOV = 0;
                SSP1CON1bits.SSPEN = 0;
                SSP1CON1bits.SSPEN = 1;
            }
                while(count<100000 && SSP1STATbits.BF == 0){
                    count++;
                }
                if(SSP1STATbits.BF == 1){
                    while(PIR1bits.SSPIF == 0){

                    }
                    PIR1bits.SSPIF = 0;
                    buf = SSP1BUF;
                    SSP1CON1bits.SSPOV ==0;
                     while(PIR1bits.SSPIF == 0){

                    }

                    PIR1bits.SSPIF = 0;
                    buf = SSP1BUF;
                    if(buf == 0x04){
                        scan = 0;
                        LCD_Goto(5,0);

                        //write string
                        LCD_WriteStr("It Works!     ", 14);
                    }
                }
        }
        PIR1bits.SSPIF = 0;
        buf = SSP1BUF;
        if(SSP1CON1bits.SSPOV == 1){
            SSP1CON1bits.SSPOV = 0;
            SSP1CON1bits.SSPEN = 0;
            SSP1CON1bits.SSPEN = 1;
        }
        while(SSP1STATbits.BF == 0){
        }
        while(PIR1bits.SSPIF == 0){

        }
        PIR1bits.SSPIF = 0;
        buf = SSP1BUF;
        
        while(SSP1STATbits.BF == 0){
            
        }
         while(PIR1bits.SSPIF == 0){

        }

        PIR1bits.SSPIF = 0;
        buf = SSP1BUF;
        SSP1STATbits.BF = 0;
        if(buf == 0x03){
            scan = 1;
        }
        else if (buf == 0x01){
            scanSensors();
            data[0] = temp%256;
            data[1] = temp/256;
            data[2]= CO2%256;
            data[3]= CO2/256;
            data[4]= salinity%256;
            data[5]= salinity/256;
            data[6]= flow%256;
            data[7]= flow/256;
            for(int i = 0; i<8; i++){
                sendI2C(data[i]);
            }
            allWrite(addr);
            addr+=8;
        }
        else if(buf == 0x02){
            int current = addr;
            for(int i = 0; i<128; i++){
                current--;
                if(current < 0){
                    current += 128;
                }
                
            }
            
        }
        
    }
    
}
