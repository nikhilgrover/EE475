#include <Wire.h>


void Examples_TestGPIO()
{
	const uint32_t mask = 1<<17;
	const uint32_t ledMask = 1<<18;


	//configure PTA17 (pin 39) as input
	PORTA_PCR17 = PORT_PCR_MUX(1);
	GPIOA_PDDR &= ~mask;

	//configure PTB18 (pin 29) as output
	//             activate pin      slew rate    drive strength  not open drain
	PORTB_PCR18 = PORT_PCR_MUX(1) | PORT_PCR_SRE | PORT_PCR_DSE  & ~PORT_PCR_ODE;
	GPIOB_PDDR |= ledMask;

	int counter = 0;
  	while(1)
  	{

		int pinState = (GPIOA_PDIR & mask) == (mask);
		if(pinState)
		{
	  		GPIOB_PDOR |= ledMask;
		}
		else
		{
	  		GPIOB_PDOR &= ~ledMask;
		}
		//printf("%d. button = %d\n", counter, pinState);
		delay(200);
		counter++;

	}
}

void Examples_TestI2C()
{
	Wire.setSDA(18);
	Wire.setSCL(19);
	Wire.begin();
	Wire.beginTransmission(0x38);
	Wire.write(0x0f);
	Wire.endTransmission();
}
