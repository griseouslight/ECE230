/************************************************************
 *    DESCRIPTION:   Use Output Compare 1 PWM to generate
 *                   a PWM wave on pin RC2 and adjust duty cycle from a pot on RA2
 *    SOURCE:        AnalogVoltagePWM.c
 *    Date:          10-25-2007
 *    AUTHOR:        Talon Lund
 *    AUTHOR:       Brad Koartge
 *********************************************************/

#include <xc.h>
#include <stdio.h>
#include "lcd4bits2015.h"

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT
//#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = ON
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config LVP = OFF

// CONFIG2
#pragma config BOR4V = BOR21V
#pragma config WRT = OFF

#define PWMPinDirection  TRISC2
#define PWM1Pin	RC2	//pin 17 of 16F887 PDIP
#define	PWM1DutyCycle CCPR1L
#define	PWM1period	PR2
#define	PWM1Flag	CCP1IF	//in PIR1 register
#define InitialDutyCyle	0x1F	//full cycle to start with
#define InitialPWMperiod    0xFF	//full period
#define ATDchannel 2
#define PHOTOchannel 3

#define LCDFirstLine 0x00
#define LCDSecondLine 0x40

#define AutoSwitch RB0

#define InitialStep 15  //15*4*8192us/1024=600us
#define NSTEPS 70   //84 to generate 2.7ms

void InitializeAnalogVoltagePWM(void);
void interrupt wave_generator(void);
void squarewave_initialization(void);
void squarewave(void);
int debounce (int button);
void auto_mode(int ATD);
void ATDInitialization(void);
void ATDsetChannel(unsigned char channel);
void ATDgetVoltage(unsigned int *Voltage);
void switch_init(void);
void manual_mode(void);


void putch(unsigned char byte) //putch() for printf()
{ lcd_putch(byte); }

/*void debug_photo() {
    ATDsetChannel(PHOTOchannel);
    lcd_goto(LCDFirstLine);
    printf("pls work :(");
}*/

void main() {
	unsigned char CurrentDutyCycle;
	unsigned int ATDvalue;

//	IRCF2=1;	IRCF1=0;	IRCF0=0;	//1 MHz
//	IRCF2=0;	IRCF1=1;	IRCF0=1;	//500 kHz
	IRCF2 = 1;	IRCF1 = 0;	IRCF0 = 1;	//2 MHz
	squarewave_initialization();
	InitializeAnalogVoltagePWM();
	PWMPinDirection = 0;	//start PWM
	switch_init();
	lcd_init();
	ATDInitialization();

	manual_mode();

//    debug_photo(); //comment out when this thing finally works

	for (;;) {
		ATDgetVoltage(&ATDvalue);
		PWM1DutyCycle = InitialStep + ATDvalue / 15;
		lcd_goto(LCDSecondLine);
		printf("Voltage=%6.4f  ", 5.0 * ATDvalue / 1024.0);
//        printf("RB0:%i", RB0
		if (debounce(AutoSwitch)) auto_mode(ATDvalue);
	}
} //end main()

void InitializeAnalogVoltagePWM() {
// initialize Timer2
	TMR2ON = 0;
	TMR2IF = 0;
	T2CKPS1 = 1;	// prescaler 1:16
	T2CKPS0 = 0;
//initialize PWM CCP1
	CCP1M3 = 1;	//PWM mode, P1A,P1C active-high, P1B,P1D active-high
	CCP1M2 = 1;
	CCP1M1 = 0;
	CCP1M0 = 0;
	PWM1DutyCycle = InitialDutyCyle;	//initial duty cycle higher byte
	PWM1period = InitialPWMperiod;
	P1M0 = 0;
	P1M1 = 0; //single output on P1A, which is CCP1
	DC1B1 = 0; //duty cycle lower bits
	DC1B0 = 0;
	TMR2ON = 1;	//turn on Timer2
}	//end of Initialize_PWM()

void interrupt wave_generator(void) {
	if (T0IF == 1) squarewave();
} //end of interrupt wave_generator()


void manual_mode(void) {
	lcd_clear();
	lcd_goto(LCDFirstLine);
	printf("Manual Mode");

	ATDsetChannel(ATDchannel);

}
#define waitTime 125
void auto_mode(int ATD) {
	lcd_clear();
	lcd_goto(LCDFirstLine);
	printf("Scanning...");

	int index = 0;
	unsigned int voltage = -1;
	ATDsetChannel(PHOTOchannel);

	PWM1DutyCycle = InitialStep;
	DelayMs(1000);


	int i;
	for (i = 0; i < NSTEPS; i++) {
		PWM1DutyCycle = InitialStep + i;
		DelayMs(waitTime);
		ATDgetVoltage(&ATD);
		if (ATD < voltage) {
			voltage = ATD;
			index = PWM1DutyCycle;
		}


	}
//   for(i=NSTEPS;i>index; i--) {
//         PWM1DutyCycle = InitialStep+i;
//         DelayMs(waitTime);
//     }


	PWM1DutyCycle = index;
	DelayMs(1000);
	lcd_goto(LCDSecondLine);
	printf("i: %i, v: %6.4f", index, 5.0 * voltage / 1024.0);


	while (!debounce(AutoSwitch));

	manual_mode();
	return;

}

int debounce(int button) {
	if (button == 0) {
		DelayMs(50);
		return (button == 0);
	}
	return 0;
}


void switch_init() {
	TRISB0 = 1;
	ANS12 = 0;
	nRBPU = 0;
	WPUB0 = 1;
}