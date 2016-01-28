// Timer0FlashLED.c
// Date: December 2015
// Jianjian Song
#include <xc.h> 

#define FlashingLED	RA0	//constantly flashing LED by timer0 
#define FlashingLEDConfig TRISA0=0; ANS0=0;

unsigned char TimeClick;    //set to be 1 each LED is toggled.

//generate a square wave on RA0
void InitializateFlashLED(void) {    
    FlashingLEDConfig;
// initialize timer0 to generate timer overflow interrupt
    T0CS = 0;   //Set clock to be Fosc/4 for Timer0
    PSA = 0;  //assign prescaler to Timer0
// set prescaler at 1:256
    PS2 = 1;   PS1 = 1;   PS0 = 1;
// set up interrupt on Timer0 overflow
	T0IF = 0;   //Clear interrupt flag
	T0IE = 1;   //turn on Timer0 interrupt
} //end InitializateFlashLED(void)

// Interrupt service routine -- Timer0 overflow interrupt
#define half_period 5
void ServeFlashLED(void)
{
static unsigned char count;
// toggle InterruptLED and clear count when count reaches half_period
    if (count==half_period) {
        count = 0;
        FlashingLED = FlashingLED+1;
        TimeClick=1;
	}
    else
        count=count+1;
    T0IF = 0;   //clear interrupt flag
} // end ServeFlashLED())
