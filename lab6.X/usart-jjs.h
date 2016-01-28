//File Name: usart-jjs.h
//modified by Jianjian Song
//constants for RS232 driver Originated from HI-TECH C
#ifndef _SERIAL_H_
#define _SERIAL_H_

#define BAUD 9600      
#define SysmteFOSC 4000000L	//20MHz. L - the constant must be assigned to a long type -- Jiajian Song

#define HIGH_SPEED 1	// 1 for high speed

#define NINE 0     /* Use 9bit communication? FALSE=8bit */

#if NINE == 1
#define NINE_BITS 0x40
#else
#define NINE_BITS 0
#endif

#if HIGH_SPEED == 1
#define SPEED 0x4
#else
#define SPEED 0
#endif

#if defined(_16F87) || defined(_16F88)
	#define RX_PIN TRISB2
	#define TX_PIN TRISB5
#else
	#define RX_PIN TRISC7
	#define TX_PIN TRISC6
#endif

void init_comms(void);
void putch(unsigned char);
unsigned char getch(void);
unsigned char getche(void);

#endif
