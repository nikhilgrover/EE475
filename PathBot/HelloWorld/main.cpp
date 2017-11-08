#include <stdarg.h>
#include "ISR_Debugger.h"
#include "WProgram.h"

#define printf Serial.printf

extern "C" int main(void)
{ 



  pinMode(29, OUTPUT);
  pinMode(30, INPUT);
  //configure PORTC_PCR5 to be an output.
  (*(volatile uint8_t *)0x43FE1294) = 1;
  //(*(volatile uint8_t *)0x43FE0848) = 1;

  //configure PORT C pin 5 as fast slew rate,
  //high drive strength, and analog input disabled (p222)
  PORTB_PCR18 = 0x0144;
  PORTC_PCR5 = 0x0144;

  uint32_t i = 0;

  Serial.begin(9600);
  Serial.flush(); 
  delay(1000);
  printf("Starting now!\n");

  while(1)
  {

        delay(500);//wait .5s

  printf("Hello from doStuff()!\n");
    //if(GPIOB_PDIR & (1<<19) == (1<<19))
    if(*portInputRegister(30))
    {
      GPIOB_PDOR|=1<<18;
    }
    else
    {
      GPIOB_PDOR&=~(1<<18);
    }
    GPIOC_PDOR |= 1<<5;//turn on pin 5 of port C (LED pin)
        delay(500);//wait .5s

  printf("Hello from doStuff()!\n");
    GPIOC_PDOR &= ~(1<<5);//turn off pin 5
    printf("%d\n",i);
    i++;
  }
}