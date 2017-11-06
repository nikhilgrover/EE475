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
    //Initiate Scan
    if(commandKey == '1'){
      char addr = '?';
      while(addr != 'q') {
	addr = '?';
	printf("Enter address in decimal, q to quit\n");
	
	while(addr < '0' || addr > '7' && addr != 'q') {
	  scanf("%c", &addr);
	}

	if(addr == 'q') {
	  printf("\nBack to main menu\n");
	}
	else {
	  addr -= 0x30; //Convert from ASCII to binary value
	  printf("\nAddress is %c\n\n", addr);
	  serialWrite(&addr);
	}
      }
    }
    //Terminate Scan
    else if(commandKey == '2') {
      char addr = '?';
      data = 0x10;
      while(addr != 'q') {
	addr = '?';
	data = 0x10;
	printf("Enter address in decimal, q to quit\n");
	
	while(addr < '0' || addr > '7' && addr != 'q') {
	  scanf("%c", &addr);
	}

	if(addr == 'q') {
	  printf("\nBack to main menu\n");
	}
	else {
	  addr -= 0x30; //Convert from ASCII to binary value
	  data += addr;
	  printf("\nData is %x\n", data);
	  serialWrite(&data);
	}
      }
    }
    //Grab reading from address
    else if(commandKey == '3') {
      char addr = '?';
      data = 0x20;
      while(addr != 'q') {
	addr = '?';
	data = 0x20;
	printf("Enter address in decimal, q to quit\n");
	
	while(addr < '0' || addr > '7' && addr != 'q') {
	  scanf("%c", &addr);
	}

	if(addr == 'q') {
	  printf("\nBack to main menu\n");
	}
	else {
	  addr -= 0x30; //Convert from ASCII to binary value
	  data += addr;
	  printf("Data being sent is %x\n\n", data);
	  serialWrite(&data);
	  //configure delay
	  serialRead();
	}
      }
    }
    else if(commandKey == '4') {
      char addr = '?';
      data = 0x30;
      while(addr != 'q') {
	addr = '?';
	data = 0x30;
	printf("Enter address in decimal, q to quit\n");
	
	while(addr < '0' || addr > '7' && addr != 'q') {
	  scanf("%c", &addr);
	}

	if(addr == 'q') {
	  printf("\nBack to main menu\n");
	}
	else {
	  addr -= 0x30; //Convert from ASCII to binary value
	  data += addr;
	  printf("Data being sent is %x\n\n", data);
	  //MAY NEED TO LOOP THIS 16 TIMES
	  serialWrite(&data);
	  //configure delay
	  serialRead();
	}
      }
    }
  }
  return 0;
}



