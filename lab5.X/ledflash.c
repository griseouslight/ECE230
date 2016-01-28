//File Name: Ledflash
//Date:		July 13, 2005
//Created by: Keith Hoover
//Modified by: Jianjian Song, Talon Lund, and Brad Koartge
// This PIC16F877 demonstration program waits until the switch on Pin RA0 is closed, then it 
// flashes the LED on Pin RC0 10 times at about a 1-second rate (flash rate depends on the clock 
//  oscillator frequency).  Then this program loops back and waits for switch to be closed once again.

#include <pic.h>  		// This header file contains symbolic register addresses
                     	// that may be used to perform input/output operations
                     	// and to access other resources on the PIC 16F877 microcontroller

#define flasher		RB0
#define flasher_direction 	TRISB0 = 0;  

//declare functions
void squarewave_initialization() {
// initialize timer0 interrupt to generate a squarewave on flasher
	T0CS = 0;   //Set clcok to be Fosc/4 for Timer0
	PSA = 0;  //assign prescaler to Timer0
// set prescaler at 1:256
	PS2 = 1;
	PS1 = 1;
	PS0 = 1;
	flasher_direction; ANS12=0;
#if defined(_16F887) 
	ANS7=0;	//RE pins are digital

#endif 
#if defined(_16F877A)
	PCFG3=0;		PCFG2=0;		PCFG1=1;		PCFG0=0;	
#endif

// set up interrupt on Timer0 overflow
	T0IF = 0;   //Clear interrupt flag
	T0IE = 1;   //turn on Timer0 interrupt
}

// Interrupt service routine -- Timer0 overflow interrupt
#define half_period 1

void squarewave()
{
static unsigned char count;
// toggle FLAG_OUTPUT and clear count when count reaches half_period
	if (count==half_period) {
		count = 0;
		flasher = flasher+1;
	}
	else
		count=count+1;
	T0IF = 0;   //clear interrupt flag
} /*end of squarewave()*/
