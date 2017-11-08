#include <stdint.h>
#include "ISR_Debugger.h"
#include "usb_dev.h"
#include "usb_serial.h"
#ifdef  __cplusplus
extern "C"
{
#endif


inline void PrintWord(uint32_t word)
{
	char opcode[8];
	char nibble = 0;
	int i;
	for(i=0;i<8;i++)
	{
		nibble = (word >> (i*4))&0x0f;
		if(nibble < 10)
			opcode[7-i] = '0' + nibble;
		else
			opcode[7-i] = 'a' - 10 + nibble;

	}
	usb_serial_write(opcode, 8);
}







inline void ConvertToHexChars(uint8_t h, char buffer[], uint64_t index)
{
	uint8_t upperN = h>>4;
	uint8_t lowerN = h&0x0f;

	//buffer[index] = upperN < 10 ? (upperN + '0') : (upperN +'a');
	//buffer[index+1] = lowerN < 10 ? (lowerN + '0') : (lowerN +'a');
	buffer[index] = upperN < 10 ? (upperN + 48) : (upperN + 97 - 10);
	buffer[index+1] = lowerN < 10 ? (lowerN + 48) : (lowerN + 97 - 10);
  buffer[index+2] = ' ';
}

#define BUFFER_LENGTH	256

//scans a section of memory and prints contents as ASCII hexadecimal
void ScanForFlashCode()
{
  uint32_t counter = 0x2002ff00;//start byte
  uint32_t end =  counter + 0x00010000;

  //buffer for flash bytes
  uint8_t buffer[BUFFER_LENGTH];

  //string buffer for hex representation of flash bytes 
  //length includes 2 chars per byte, plus spacing between each byte
  char strBuffer[BUFFER_LENGTH * 3];

  //loop through all bytes to be printed
  while(counter<end)
  {
  	//loop through 1 buffer length of data, and convert to a char[]
  	int i;
    for (i = 0; i<BUFFER_LENGTH;i++)
    {
      //use counter as an address pointer and store in buffer[i]
      buffer[i] = *((volatile uint8_t*)(counter));
      //convert byte to ASCII so COM terminal can show data
      ConvertToHexChars(buffer[i], strBuffer, i*3);
      counter++;
    }
    //send char[] over serial
    usb_serial_putchar('\n');
    usb_serial_write(strBuffer, BUFFER_LENGTH * 3);
  }
}

#define BUFSIZE 64
uint32_t sp[BUFSIZE];
uint32_t stackSize = 0;
extern void StackAccess(unsigned long *sptr)
{
	
	int i;
	for(i = 0;i<BUFSIZE;i++)
	{
		sp[i] = sptr[i];
		if(sptr[i] == 0xffffffff)
			break;
	}
	stackSize = ++i;
	
	//for(i=0;i<4;i++)
	//	sp[stackSize-4+i] = (0x000000ff >> (i*8)) & stackSize;

}


static inline uint32_t* GetSP()
{
  uint32_t* sp;
  asm volatile ("MOV r7,%0 \n \
                 MOV r8, sp \n \
                  STR r8,[r7]\
                  " : : "r" (&sp) : "r7","r8");
  return sp;
}

extern void SendData()
{
	int c;
	c = usb_serial_getchar();
	// usb_serial_putchar(c);
	// usb_serial_putchar(c);

	// usb_serial_putchar(c);
	// usb_serial_putchar(c);
	if(c != '$')
		return;

	if(c == '%')
		while(1){}

	int i;
	for(i=0;i<stackSize;i++)
	{
		//PrintWord(__builtin_extract_return_addr(sp[i]));
		PrintWord(sp[i]);
		usb_serial_putchar(' ');	
	}
	usb_serial_putchar('\n');

	usb_serial_flush_input();
}





#ifdef  __cplusplus
}
#endif