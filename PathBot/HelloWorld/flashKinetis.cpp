#include "flashKinetis.h"

#define FCMD_READ_1S_BLOCK          	0x00
#define FCMD_READ_1S_SECTION        	0x01
#define FCMD_PROGRAM_CHECK          	0x02
#define FCMD_READ_RESOURCE          	0x03
#define FCMD_PROGRAM_LONG_WORD      	0x06
#define FCMD_ERASE_FLASH_BLOCK      	0x08
#define FCMD_ERASE_FLASH_SECTOR     	0x09
#define FCMD_PROGRAM_SECTOR         	0x0b
#define FCMD_READ_1S_ALL_BLOCKS     	0x40
#define FCMD_READ_ONCE              	0x41
#define FCMD_PROGRAM_ONCE           	0x43
#define FCMD_ERASE_ALL_BLOCKS       	0x44
#define FCMD_SET_FLEXRAM				0x81

#define FCMD_SET_FLEXRAM_EEPROM			0x00
#define FCMD_SET_FLEXRAM_RAM			0xff

#define FTFL_STAT_MGSTAT0 				0x01	// Error detected during sequence
#define FTFL_STAT_FPVIOL  				0x10	// Flash protection violation flag
#define FTFL_STAT_ACCERR  				0x20	// Flash access error flag
#define FTFL_STAT_CCIF    				0x80	// Command complete interrupt flag

#define FTFL_READ_MARGIN_NORMAL			0x00

/*
  ==================================================================
	Internal Functions
  ==================================================================
*/

#define FLASH_ALIGN(address,align) address=((unsigned long*)((unsigned long)address & (~(align-1))))

static void (*fnRAM_code)(volatile unsigned char *) = 0;

FASTRUN static void flashExec(volatile uint8_t *fstat) 								// Execute Flash Command in RAM
{																					
	*fstat = FTFL_STAT_CCIF;
	while (!(*fstat & FTFL_STAT_CCIF)) {;}
}

static void flashInitCommand(unsigned char command, unsigned long *address)
{
	while (!(FTFL_FSTAT & FTFL_STAT_CCIF)) {;}
	FTFL_FSTAT  = 0x30;
	FTFL_FCCOB0 = command;
	FTFL_FCCOB1 = ((unsigned long)address) >> 16;
	FTFL_FCCOB2 = ((unsigned long)address) >> 8;
	FTFL_FCCOB3 = ((unsigned long)address);
}

/*
  ==================================================================
	User Functions
  ==================================================================
*/

int flashCheckSectorErased(unsigned long *address) 									// Check if sector is erased
{
	FLASH_ALIGN(address, FLASH_SECTOR_SIZE);
	flashInitCommand(FCMD_READ_1S_SECTION, address);
	const int num = FLASH_SECTOR_SIZE >> 2;
	FTFL_FCCOB4 = (unsigned char)(num >> 8);										// Number of longwords
	FTFL_FCCOB5 = (unsigned char)(num);
	FTFL_FCCOB6 = FTFL_READ_MARGIN_NORMAL;
	__disable_irq();
	flashExec(&FTFL_FSTAT);
	__enable_irq();
	return (FTFL_FSTAT & (FTFL_STAT_ACCERR | FTFL_STAT_FPVIOL | FTFL_STAT_MGSTAT0));
}

int flashEraseSector(unsigned long *address, bool allowFirstSector)					// Erase Flash Sector
{
	FLASH_ALIGN(address, FLASH_SECTOR_SIZE);
	if((unsigned long)address<FLASH_SECTOR_SIZE) if(!allowFirstSector) return -1;	// Won't erase first sector unless flagged end user is sure.
	if (flashCheckSectorErased(address)  && (address > 0) )
		{
			flashInitCommand(FCMD_ERASE_FLASH_SECTOR, address);
			__disable_irq();
			flashExec(&FTFL_FSTAT);
			__enable_irq();
			return (FTFL_FSTAT & (FTFL_STAT_ACCERR | FTFL_STAT_FPVIOL | FTFL_STAT_MGSTAT0));			
		}
	return 0;
}

int flashProgramWord(unsigned long *address, unsigned long *data, bool aFS, bool oSFC)	// Program Flash, one long word (32 Bit)
{
	FLASH_ALIGN(address, 0x04);
	if((unsigned long)address<FLASH_SECTOR_SIZE) if(!aFS) return -1;		// no writing in first sector unless end user is sure.
	if (((((unsigned long)address)>=0x400) && ((unsigned long)address)<=0x40C))
		if(!oSFC) return -2;														// Make sure not to write 0x400 - 0x40F
	flashInitCommand(FCMD_PROGRAM_LONG_WORD, address);
	FTFL_FCCOB4 = (unsigned char)(*data >> 24);            							// Enter the long word to be programmed
	FTFL_FCCOB5 = (unsigned char)(*data >> 16);
	FTFL_FCCOB6 = (unsigned char)(*data >> 8);
	FTFL_FCCOB7 = (unsigned char)*data;
	__disable_irq();
	fnRAM_code((volatile unsigned char *)FTFL_BLOCK);                    // execute the command from SRAM
	//flashExec(&FTFL_FSTAT);
	__enable_irq();
	return (FTFL_FSTAT & (FTFL_STAT_ACCERR | FTFL_STAT_FPVIOL | FTFL_STAT_MGSTAT0));
}

// Disable EEPROM function and enable use of FLEXRAM as RAM
void flashSetFlexRAM(void)
{
	while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF)) {;}
	FTFL_FSTAT  = 0x30;
	FTFL_FCCOB0 = FCMD_SET_FLEXRAM;
	FTFL_FCCOB1 = FCMD_SET_FLEXRAM_RAM;
	flashExec(&FTFL_FSTAT);
}

unsigned long flashFirstEmptySector(void)
{
  uintptr_t adr = 3 * FLASH_SECTOR_SIZE;
  while (flashCheckSectorErased((unsigned long*)adr)) adr += FLASH_SECTOR_SIZE;
  return adr;
}  

/* FTFL_FSEC
B7-B6 KEYEN Backdoor Key Security Enable
  These bits enable and disable backdoor key access to the flash memory module.
  00 Backdoor key access disabled
  01 Backdoor key access disabled (preferred KEYEN state to disable backdoor key access)
  10 Backdoor key access enabled
  11 Backdoor key access disabled (Default)
B5-B6 MEEN Mass Erase Enable Bits
  Enables and disables mass erase capability of the flash memory module. The state of the MEEN bits is
  only relevant when the SEC bits are set to secure outside of NVM Normal Mode. When the SEC field is
  set to unsecure, the MEEN setting does not matter.
  00 Mass erase is enabled
  01 Mass erase is enabled
  10 Mass erase is disabled
  11 Mass erase is enabled
B3–B2 FSLACC Freescale Failure Analysis Access Code
  These bits enable or disable access to the flash memory contents during returned part failure analysis at
  Freescale. When SEC is secure and FSLACC is denied, access to the program flash contents is denied
  and any failure analysis performed by Freescale factory test must begin with a full erase to unsecure the
  part. When access is granted (SEC is unsecure, or SEC is secure and FSLACC is granted), Freescale factory
  testing has visibility of the current flash contents. The state of the FSLACC bits is only relevant when the
  SEC bits are set to secure. When the SEC field is set to unsecure, the FSLACC setting does not matter.
  00 Freescale factory access granted
  01 Freescale factory access denied
  10 Freescale factory access denied
  11 Freescale factory access granted
B1–B0 SEC Flash Security
  These bits define the security state of the MCU. In the secure state, the MCU limits access to flash
  memory module resources. The limitations are defined per device and are detailed in the Chip
  Configuration details. If the flash memory module is unsecured using backdoor key access, the SEC bits
  are forced to 10b.
  00 MCU security status is secure
  01 MCU security status is secure
  10 MCU security status is unsecure (The standard shipping condition of the flash memory module is unsecure.)
  11 MCU security status is secure
    FTFL_FSEC=0x64; // 0b1100100 = 100 = the value robsoles will recommend locking up with.
  
*/



int flashSecurityLockBits(uint8_t newValueForFSEC)
{
  while (!(FTFL_FSTAT & FTFL_STAT_CCIF)) {;}
	FTFL_FSTAT  = 0x30;
	FTFL_FCCOB0 = FCMD_PROGRAM_LONG_WORD;
	FTFL_FCCOB1 = 0;
	FTFL_FCCOB2 = 4;
	FTFL_FCCOB3 = 0xC;
  
	FTFL_FCCOB4 = 0xFF; // It is not possible to turn bits on without erasing a larger block, I am
	FTFL_FCCOB5 = 0xFF; // using all on value 
	FTFL_FCCOB6 = 0xFF; // 
	FTFL_FCCOB7 = newValueForFSEC;
	__disable_irq();
  flashExec(&FTFL_FSTAT);
	__enable_irq();
	return (FTFL_FSTAT & (FTFL_STAT_ACCERR | FTFL_STAT_FPVIOL | FTFL_STAT_MGSTAT0));
}

void flashQuickUnlockBits()
{
  while (!(FTFL_FSTAT & FTFL_STAT_CCIF)) {;}
	FTFL_FSTAT  = 0x30;
	FTFL_FCCOB0 = FCMD_ERASE_FLASH_SECTOR;
	FTFL_FCCOB1 = 0;
	FTFL_FCCOB2 = 0;
	FTFL_FCCOB3 = 0;

	__disable_irq();
	FTFL_FSTAT = FTFL_STAT_CCIF;
	while (!(FTFL_FSTAT & FTFL_STAT_CCIF)) {;}
	FTFL_FSTAT  = 0x30;
	FTFL_FCCOB0 = FCMD_PROGRAM_LONG_WORD;
	FTFL_FCCOB1 = 0;
	FTFL_FCCOB2 = 4;
	FTFL_FCCOB3 = 0xC;
  
	FTFL_FCCOB4 = 0xFF;
	FTFL_FCCOB5 = 0xFF;
	FTFL_FCCOB6 = 0xFF;
	FTFL_FCCOB7 = 0xFE;
	FTFL_FSTAT = FTFL_STAT_CCIF;
	while (!(FTFL_FSTAT & FTFL_STAT_CCIF)) {;}
}

// This routine runs from SRAM - the reason why the pointer is passed is to avoid the routine taking it from a const value in FLASH, which is then not code location dependent
//
static unsigned short fnFlashRoutine[] = {                               // to avoid potential compiler in-lining of the routine (removing position independency) the machine code is used directly
    0x2180,    // MOVS   r1,#0x80                                           load the value 0x80 (command complete interrupt flag) to register r1
    0x7001,    // STRB   r1,[r0,#0x00]                                      write r1 (0x80) to the passed pointer location (r0)
    0x7801,    // LDRB   r1,[r0,#0x00]                                      read back from the same location to r1
    0x0609,    // LSLS   r1,r1,#24                                          shift the register content by 24 bits to the left so that the command complete interrupt flag is at bit 31
    0xd5fc,    // BPL    -4                                                 if the command complete interrupt flag bit is '0' (register content is not negative value) branch back to read its value again
    0x4770     // BX     lr                                                 return from sub-routine
};


void makeRAMCmd()
{
        #define PROG_WORD_SIZE 30                                        // adequate space for the small program
        int i = 0;
        unsigned char *ptrThumb2 = (unsigned char *)fnFlashRoutine;
        static unsigned short usProgSpace[PROG_WORD_SIZE];               // make space for the routine on stack (this will have an even boundary)
 
        ptrThumb2 =  (unsigned char *)(((CAST_POINTER_ARITHMETIC)ptrThumb2) & ~0x1); // thumb 2 address
        while (i < PROG_WORD_SIZE) {                                     // copy program to SRAM
            usProgSpace[i++] = *(unsigned short *)ptrThumb2;
            ptrThumb2 += sizeof (unsigned short);
        }
        ptrThumb2 = (unsigned char *)usProgSpace;
        ptrThumb2++;                                                     // create a thumb 2 call
        fnRAM_code = (void(*)(volatile unsigned char *))(ptrThumb2);
}