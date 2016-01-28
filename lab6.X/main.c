/*
 * File:   lab6template.c
 * Author: song, Talon Lund (CM406), Brad Koartge (CM365)
 *
 * This is a recommended approach to lab 6
 *
 * Created on January 26, 2016, 8:06 PM
 */

#include <stdio.h>
#include <xc.h>
#include "usart-jjs.h"


// CONFIG1
//#pragma config FOSC =   HS
#pragma config FOSC = INTRC_NOCLKOUT  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR21V   // Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


//define all hardware interface pins
#define KEYPAD  PORTB
/*4x4 black keypad, RB4, 6, 7, 0 are input rows 0-3 and RB1, 2, 3, 5 are output columns 0-3
#define KEYPAD_DIRECTION TRISB = 0b11010001 //RB6 and RB7 are used by PICkit 3
#define KEYPAD_PULLUP nRBPU=0; WPUB = 0b11010001    //enable and add pullup resistors to all input pins
*/
//4x4 white keypad, RB3-0 are input rows 3-0 and RB7-4 are output columns 3-0
#define KEYPAD_DIRECTION TRISB = 0b00001111;    //RB6 and RB7 are used by PICkit 3
#define KEYPAD_PULLUP nRBPU=0; WPUB = 0b00001111;   //enable and add pullup resistors to all input pins
#define MakingPortbDigital  ANSELH = 0b00000000;    //make PORTB RB5-0 digital
#define LED PORTD   //bargraph
#define LED_DIRECTION TRISD = 0b00000000;

#define RED_LED RD0
#define GREEN_LED RD1
#define BLUE_LED RD2

#define cfg_LED TRISD0 = 0; TRISD1 = 0; TRISD2 = 0;




void squarewave_initialization(void);
void EnterSecuryKey(unsigned char *FiveHexNumbers);
void FindSwitch(unsigned char *KeyNumber);
void DisplayKey(unsigned char key);
void DelayMs(unsigned int nrms);
void hardware_init(void);
void KeyToChar(char* Key);

const unsigned char Title[] = {"Lab 6!"};

void main() {

	unsigned char SecurityCode[5];  //store security key
	unsigned char key; char i;
	init_comms();
//    squarewave_initialization();
	hardware_init();

	printf("\n\r%s\n\r", Title);
	//Initialize variables and port pins;

	RED_LED = 1;
	GREEN_LED = 1;
	BLUE_LED = 0;


	EnterSecuryKey(SecurityCode);

	RED_LED = 1;
	GREEN_LED = 0;
	BLUE_LED = 0;

	char j;
	printf("\n\rCode: ");
	for (j = 0; j < 5; j++) {
		printf("%c ", SecurityCode[j], SecurityCode[j]);
	}

	printf("\n\rEntering: \n\r");
	char working = 0; //boolean
	while (1) { //enter five digit key from the keypad
		for (i = 0; i < 5; i++) {
			key = 0;
			while (key == 0) FindSwitch(&key);
			RED_LED = 1;
			GREEN_LED = 0;
			BLUE_LED = 0;
			KeyToChar(&key);
//            DisplayKey(key);
//            printf("\33[2K");
			printf("%c ", key);
//            if (key!=SecurityCode[i]) break;
			if (i == 0 && key == SecurityCode[i]) working = 1;
			if (key != SecurityCode[i]) working = 0;

		}
//        while (key == 0) FindSwitch(&key);
		if (working) {
			printf("\r\nGRANTED.");
			RED_LED = 0;
			GREEN_LED = 1;
			BLUE_LED = 0;
		} else {
			printf("\r\nDENIED.");
			RED_LED = 1;
			GREEN_LED = 0;
			BLUE_LED = 0;
		}
		printf("\r\n");
	}
}//end main()

const char Validkeys[16] = {'0', '1', '2', '3',
                            '4', '5', '6', '7',
                            '8', '9', 'a', 'b',
                            'c', 'd', 'e', 'f'
                           };

void EnterSecuryKey(unsigned char *FiveHexNumbers) {
	char i = 0, j, keyword;
	//Enter the five-digit security code through serial port;
	//  Display key numbers on serial terminal;
	//  Try again if a number is not a hexadecimal number;
	//  Store the five-digit key in FiveHexNumbers[5];
	printf("\r\n");

	while (i < 5) {
		keyword = getch();
		printf("\33[2K");
		printf("\rEntered: %c", keyword);
		for (j = 0; j < 16; j++) {
			if (keyword == Validkeys[j]) { //matched
				FiveHexNumbers[i] = keyword;
				printf(" ... Stored!");
				i++;
				break;
			}
		}
	}

}

void KeyToChar (char *Key) {
	*Key = Validkeys[(*Key) - 1];
}

const char KeyPatterns[16] = {/*0*/ 0b10111110,
                                    /*1*/ 0b01110111,
                                    /*2*/ 0b10110111,
                                    /*3*/ 0b11010111,
                                    /*4*/ 0b01111011,
                                    /*5*/ 0b10111011,
                                    /*6*/ 0b11011011,
                                    /*7*/ 0b01111101,
                                    /*8*/ 0b10111101,
                                    /*9*/ 0b11011101,
                                    /*A*/ 0b11100111,
                                    /*B*/ 0b11101011,
                                    /*C*/ 0b11101101,
                                    /*D*/ 0b11101110,
                                    /*E*/ 0b01111110, //*
                                    /*F*/ 0b11011110  //#
                             };

#define KEYPAD PORTB
void FindSwitch(unsigned char *KeyNumber) {
	char i;
	*KeyNumber = 0;
	for (i = 0; i < 16; i++) {
		KEYPAD = KeyPatterns[i];
		DelayMs(10);
		if (KEYPAD == KeyPatterns[i]) {
			DelayMs(10);
			if (KEYPAD == KeyPatterns[i]) {
				*KeyNumber = i + 1;
			}
			while (KEYPAD == KeyPatterns[i]);
			return;
		}
	}
}

void DisplayKey(unsigned char Key) {
	printf("Pressed: %i", Key);
}


void DelayMs(unsigned int nrms) {
	unsigned int i, j;
	for (j = 0; j < nrms; j++)
		for (i = 0; i < 20; i++);
}


void hardware_init(void) {
	KEYPAD_DIRECTION;
	KEYPAD_PULLUP;
	MakingPortbDigital;
	LED_DIRECTION;
	cfg_LED;
//    OSCON=0B01110000;

	RED_LED = 1;
	GREEN_LED = 1;
	BLUE_LED = 1;
	DelayMs(30);

}