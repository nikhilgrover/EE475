#ifndef WProgram_h
#define WProgram_h

#include <stdlib.h>
#include <string.h>
#include <math.h>

// some libraries and sketches depend on this
// AVR stuff, assuming Arduino.h or WProgram.h
// automatically includes it...
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "avr_functions.h"
#include "HardwareSerial.h"
#define DMAMEM __attribute__ ((section(".dmabuffers"), used))
#define FASTRUN __attribute__ ((section(".fastrun"), noinline, noclone ))


#include "avr_emulation.h"
#include "usb_serial.h"

uint16_t makeWord(uint16_t w);
uint16_t makeWord(char h, char l);

#define word(...) makeWord(__VA_ARGS__)


#include "pins_arduino.h"




extern "C" {
extern void *memcpy (void *dst, const void *src, size_t count);
}
#else
extern void *memcpy (void *dst, const void *src, size_t count);



#endif // WProgram_h
