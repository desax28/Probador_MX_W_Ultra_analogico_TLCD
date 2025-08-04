#ifndef TLCD_LEDS_H
#define	TLCD_LEDS_H


#include "project.h"

void tlcd_leds_init (void);

void tlcd_leds_setLed (u8 led);
void tlcd_leds_clearLed (u8 led);
void tlcd_leds_toggleLed (u8 led);

u8 tlcd_leds_getLed (u8 led);


#endif	/* TLCD_LEDS_H */

