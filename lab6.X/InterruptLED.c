//File Name: InterruptLED.c
//Date:		August 2009
//Written by: Jianjian Song
// A LED on Pin RD2 is turned on and off once every second by a Timer0 interrupt service.

#include <xc.h>	//Hi-Tech compiler will choose the correct processor header file to use. 

#define FlashingLED		RD2	//constantly flashing LED
unsigned char NextLine;	//print next line

void squarewave_initialization(void) {
	TRISD2 = 0;
// initialize timer0 interrupt to generate a squarewave on flasher
	T0CS = 0;   //Set clcok to be Fosc/4 for Timer0
	PSA = 0;  //assign prescaler to Timer0
// set prescaler at 1:256
	PS2 = 1;
	PS1 = 1;
	PS0 = 1;
// set up interrupt on Timer0 overflow
	T0IF = 0;   //Clear interrupt flag
	T0IE = 1;   //turn on Timer0 interrupt
	GIE = 1;   //turn on global interrupt flag
}

// Interrupt service routine -- Timer0 overflow interrupt
#define half_period 15
void interrupt squarewave(void)
{
static unsigned char count;
// toggle FLAG_OUTPUT and clear count when count reaches half_period
	if (count==half_period) {
		count = 0;	NextLine=1;
		FlashingLED = FlashingLED+1;
	}
	else
		count=count+1;
	T0IF = 0;   //clear interrupt flag
} /*end of squarewave()*/
