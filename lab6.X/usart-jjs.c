//File Name: usart-jjs.c
//modified by Jianjian Song
//RS232 driver originated from HI-TECH

#include <xc.h>
#include <stdio.h>
#include "usart-jjs.h"

//DIVIDER generates the baud rate register value 
#define DIVIDER ((int)(SysmteFOSC/(16UL * BAUD) -1))	//16UL unsigned long type	-- Jianjian Song

void init_comms(void) {
//* Serial initialization */
	RX_PIN = 1;	
	TX_PIN = 1;
	SPBRG = DIVIDER;
	BAUDCTL=0;
	RCSTA = (NINE_BITS|0x90);
	TXSTA = (SPEED|NINE_BITS|0x20);
}
void 
putch(unsigned char byte) 
{
	/* output one byte */
	while(!TXIF)	/* set when register is empty */
		continue;
	TXREG = byte;
}

unsigned char 
getch() {
	/* retrieve one byte */
	while(!RCIF)	/* set when register is not empty */
		continue;
	return RCREG;	
}

unsigned char
getche(void)
{
	unsigned char c;
	putch(c = getch());
	return c;
}
