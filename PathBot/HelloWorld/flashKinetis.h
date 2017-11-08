

#ifndef flashKinetis_h_
#define flashKinetis_h_


#define FLASH_SIZE			(1 * 1024 * 1024)
#define FLASH_SECTOR_SIZE	 (4 * 1024) 
#define FTFL_BLOCK                         0x40020000                // Flash Memory Module
#define CAST_POINTER_ARITHMETIC unsigned long                            // Kinetis uses 32 bit pointers

#include "kinetis.h"
#include "WProgram.h"


int flashCheckSectorErased(unsigned long *address);
int flashEraseSector(unsigned long *address, bool allowFirstSector=false);
int flashProgramWord(unsigned long *address, unsigned long *data, bool allowFirstSector=false, bool overrideSafetyForConfig=false);
void flashSetFlexRAM(void);
unsigned long flashFirstEmptySector(void); //returns first unused sector

int flashSecurityLockBits(uint8_t newValueForFSEC=0x64); // 0x64 appears to be one of the safer values to use here.
void flashQuickUnlockBits();

void makeRAMCmd();
#endif
