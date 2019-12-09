/*
 * mesh_leds.c
 *
 *  Created on: 1 Dec 2019
 *      Author: Prinzessin
 *
 *      LED0: small red
 *      LED1: big red
 *      LED2: big green
 *      LED3: big blue
 */

#include <stdio.h>
#include <stdint.h>

#include "board.h"
#include "periph_conf.h"

void led_switch_off_all(void);
void led_switch(char colour);


#ifdef CLOCK_CORECLOCK
#define DELAY_SHORT         (CLOCK_CORECLOCK / 50)
#else
#define DELAY_SHORT         (500000UL)
#endif
#define DELAY_LONG          (DELAY_SHORT * 4)

void dumb_delay(uint32_t delay) {
	for (uint32_t i = 0; i < delay; i++) {
		__asm__("nop");
	}
}

void led_test(void){
	led_switch('R');
	dumb_delay(DELAY_LONG);
	led_switch('G');
	dumb_delay(DELAY_LONG);
	led_switch('B');
	dumb_delay(DELAY_LONG);
	led_switch('W');
	dumb_delay(DELAY_LONG);
	led_switch('C');
	dumb_delay(DELAY_LONG);
	led_switch('M');
	dumb_delay(DELAY_LONG);
	led_switch('Y');
	dumb_delay(DELAY_LONG);
	led_switch('K');
	dumb_delay(DELAY_LONG);
	led_switch('O');
	dumb_delay(DELAY_LONG);
}

void led_switch_off_all(void) {
#ifdef LED0_ON
	LED0_OFF;
#endif
#ifdef LED1_ON
	LED1_OFF;
#endif
#ifdef LED2_ON
	LED2_OFF;
#endif
#ifdef LED3_ON
	LED3_OFF;
#endif

}

void led_switch(char colour) {
	led_switch_off_all();
	if (colour == 'R') {
#ifdef LED1_ON // red
		LED1_ON;
#endif
	} else if (colour == 'G') {
#ifdef LED2_ON // green
		LED2_ON;
#endif
	} else if (colour == 'B') {
#ifdef LED3_ON // blue
		LED3_ON;
#endif
	} else if (colour == 'W') {
#ifdef LED1_ON // white
		LED1_ON;
#endif
#ifdef LED2_ON
		LED2_ON;
#endif
#ifdef LED3_ON
		LED3_ON;
#endif
	} else if (colour == 'C') {
#ifdef LED2_ON // cyan
		LED2_ON;
#endif
#ifdef LED3_ON
		LED3_ON;
#endif
	} else if (colour == 'M') {
#ifdef LED1_ON // magenta
		LED1_ON;
#endif
#ifdef LED3_ON
		LED3_ON;
#endif
	} else if (colour == 'Y') {
#ifdef LED1_ON // yellow
		LED1_ON;
#endif
#ifdef LED2_ON
		LED2_ON;
#endif
	} else if (colour == 'O') {
		// black
		led_switch_off_all();
	} else { // Internal Error

#ifdef LED0_ON
		LED0_ON;
#endif
	}
}
