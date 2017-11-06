#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h> 
#include <termios.h> 
#include <unistd.h>
#include <errno.h>
#include "serialRead.h"
#include "serialWrite.h"

int main(int argc, char *argv[])
{
  char commandKey = '?';
  unsigned char data;
  while(commandKey != '0') {
    printf("Type in one of the following commands:\n 0: Quit\n 1: Initiate a scan\n 2: Terminate scan\n 3: Grab the last reading from a particular system\n 4: Grab the last 16 readings from a particular system\n");
    commandKey = '?';

    while(commandKey < '0' || commandKey > '5') {
      scanf("%c", &commandKey);
    }

    if(commandKey == '0') {
      printf("Exiting program...\n");
      return 0;
    }
    if(commandKey == '1'){
      printf("Enter the address of the system you'd like to scan in binary:\n");
      data = (0x0 << 4); //Shift instruction over by 4, so sys addr can be added
      unsigned char addr;
      scanf("%c", &addr);
      data += addr;
      serialWrite(&data);
    }
    else if(commandKey == '2') {
      printf("Enter the address of the system you'd like to terminate in binary:\n");
      data = (0x1 << 4);
      unsigned char addr;
      scanf("%c", &addr);
      data += addr;
      serialWrite(&data);
    }
    else if(commandKey == '3') {
      printf("Enter the address of the system you'd like to read in binary:\n");
      data = (0x2 << 4);
      unsigned char addr;
      scanf("%c", &addr);
      data += addr;
      serialWrite(&data);
      //add some delay here
      serialRead();
      // Process data into floats and print
    }
    else if(commandKey == '4') {
      printf("Enter the address of the system you'd like to read in binary:\n");
      data = (0x3 << 4);
      unsigned char addr;
      scanf("%c", &addr);
      data += addr;
      serialWrite(&data);
      //delay
      serialRead();
      //Process data into floats and print
    }
  }
}



