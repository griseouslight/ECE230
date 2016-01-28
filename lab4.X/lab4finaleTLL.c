/* lab4part1Player_template.c
   ECE230 Lab #4 Part 1 Template Interrupt-driven tone generation Timer 1 and CCP1
 * Author: Jianjian Song
 * Date: December 2015
 * Based a program by Professors Keith Hoover and Chris Miller
 * Input: Pushbutton on RB0 to start the 440Hz square wave on RC2 for note A4
 * outputs: flashing LED on RA0. 440Hz square wave on RC2

   This PIC16F887 demonstration program shows how to use the PIC16F887 "Compare and Capture Module #1" (CCP1)operating in its
   "compare mode" along with Hardware Timer 1 to generate a square wave of frequency 440 Hz (Middle A on the piano) on the CCP1 pin.

   The function "InitializeCCP1( )" is called to initialize Timer 1 and output compare channel 1 CCP1 to toggle pin RC2.
   First timer 1 is initialized for a tick rate of (4/Fosc)*1 = 1 MHz, assuming that we are using the internal RC clock oscillator whose
   default oscillator frequency is Fosc = 4 MHz.  This yields a Timer 1 tick period of 1 / 1 MHz = 1 microsecond.

   Compare and Capture Unit #1 is initialized into its "Interrupt Upon Compare and Toggle CCP1 Output Pin" mode, where the present value
   of Timer 1 is read and the number of ticks to be waited (half of a 440 Hz period, 1 MHz / (2*440) = 1136 timer ticks is placed into the
   CCPR1 register.  The CCPR1 register is compared continuously with TIMER 1.  When there is a match, 1/2 of a 440 Hz has been waited out,
   and a CCP1 interrupt occurs and the CCP1 pin is automatically toggled, thereby generating a 440 Hz square wave on pin RC2.

*/
#include <xc.h>
// #include <xc.h>
#include <stdio.h>
#include "lcd8bits2015.h"
//Chip type         :PIC16F887
//Clock frequency   :4 MHz

// CONFIG1
//#pragma config FOSC = HS    // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       //   External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR21V   // Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
//hardware definition
#define OnOffSwitch RB0
#define OnOffSwitchConfig TRISB=0b00001111; ANS12=0; ANS10=0; ANS9=0; ANS8=0; WPUB=0b00001111; nRBPU=0;
#define Keyboard    PORTB
#define StopPlay    CCP1M1 = 0;
#define StartPlay   CCP1M1 = 1;
#define Speaker RC2 //Output Compare pin action, output by default

//constant definitions
#define Fosc 3675000    //calibrated to measure A4 correct
//half period count= Fosc/(4*2*440)=4000000/ (4*2*440) = 1136
#define A4 440.00   //Hz Note A4 in Hz
#define A4count Fosc/(8*A4) //generate count at compile time for speed
#define ON 0
#define OFF 1


//subroutine definitions
void InitializeCCP1(void);
void ServeCCP1(void);
void InitializateFlashLED(void);
void ServeFlashLED(void);
void interrupt interrupt_handler(void);



//subroutine definitions
void InitializeCCP2(void);
void ServeCCP2(void);
void InitializateFlashLED(void);
void ServeFlashLED(void);
void interrupt interrupt_handler(void);



//constant definitions
//Internal 4MHz oscillator is not exactly 4MHz
#define Fosc 1007000*4    //measured frequency on pin 14 RA6
//half period count= Fosc/(4*2*440)=4000000/ (4*2*440) = 1136
//half period count= Fosc/(4*2*440)=3699000/ (4*2*440) = 1050


#define A3 220.00
#define A3count Fosc/(8*A3)

#define B3 246.94
#define B3count Fosc/(8*B3)

#define C4 261.63
#define C4count Fosc/(8*C4)

#define A4 440.00   //Frequency in Hz of Note A4
//#define A4count 1044 //Fosc/(8*A4) //generate count at compile time for speed
#define A4count Fosc/(8*A4)

#define B4 493.88
#define B4count Fosc/(8*B4)

#define C5 523.26
#define C5count Fosc/(8*C5)

// CONFIG1
//#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
//#pragma config FOSC = HS
//#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
//#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
//#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
//#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
//#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
//#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
//#pragma config BOR4V = BOR21V   // Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
//#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
//hardware definition
#define FlashingLED RA0 //constantly flashing LED by timer0 
#define FlashingLEDConfig TRISA0=0; ANS0=0;
#define TunerLED    RE0 //constantly flashing LED by timer0 
#define TunerLEDConfig TRISE0=0; ANS5=0;


//global variables
unsigned int CCP1Count; //CCP1 count for half a period
unsigned long int Beats; //number of beats
//sound sheet and note array. Test sound: (duration,note);
const int TestSound[4][2] = {200, 0, 100, 1, 0, 0};
const unsigned int notes[] = {A3count, A4count};


//global variables
unsigned int period; //period in number of counts
const unsigned char Title[] = {"CCP2 Period"};
//convert putch from uart subroutine to LCD subroutine
extern unsigned char TimeClick;

void main(void)
{
    OSCCON = 0b01100001; //internal oscillator at 4MHz
    unsigned char bar;
    InitializateFlashLED();
    InitializeCCP1();
    OnOffSwitchConfig;
    GIE = 1;   //turn on global interrupt flag
    bar = 0;
    while (TestSound[bar][0] != 0) {
        CCP1Count = notes[TestSound[bar][1]];
        Beats = TestSound[bar][0];
        while (Beats != 0) {};
        bar = bar + 1;
    }


    //2
    float Frequency;
    InitializateFlashLED();
    InitializeCCP2();
    TunerLEDConfig;
    lcd_init(); //8-bit node
    lcd_clear();    //clear LCD
    lcd_goto(0x40); // select second line
    printf("%s", "CCP2 Period");
//  lcd_puts(Title);
    period = (int) A4count * 2;
    GIE = 1;   //Enable global interrupt flag


    CCP1Count = A4count;
    while (1) {         //check switch
        switch (Keyboard & 0b00001111) {
        case 0b00001101: {StartPlay; CCP1Count = A3count; break;}
        case 0b00001111: {StopPlay; CCP1Count = 0; break;}
        case 0b00001011: {StartPlay; CCP1Count = B3count; break;}
        case 0b00000111: {StartPlay; CCP1Count = C4count; break;}
        case 0b00001100: {StartPlay; CCP1Count = A4count; break;}
        case 0b00001010: {StartPlay; CCP1Count = B4count; break;}
        case 0b00000110: {StartPlay; CCP1Count = C5count; break;}
        default: {StopPlay; CCP1Count = 0; break;}
        }

        //2
        if (TimeClick == 1) {
            lcd_goto(0x00);
            Frequency = (float) Fosc / (4 * period);
            printf("%s %4.1f  %s", "Freq: ", Frequency, "Hz   ");
            TimeClick = 0;  //reset time reference
        } //end if(TimeClick==1)
        //Turn on the LED on tuner if frequency is below 440 Hz
        if (Frequency > A4) TunerLED = ON;   else TunerLED = OFF;


    } //end while(1)
} //end main()


// void main(void)
// {
//     float Frequency;
//     InitializateFlashLED();
//     InitializeCCP2();
//     TunerLEDConfig;
//     lcd_init(); //8-bit node
//     lcd_clear();    //clear LCD
//     lcd_goto(0x40); // select second line
//     printf("%s", "CCP2 Period");
// //  lcd_puts(Title);
//     period=(int) A4count*2;
//     GIE = 1;   //Enable global interrupt flag
//     while(1){
//         if(TimeClick==1) {
//             lcd_goto(0x00);
//             Frequency=(float) Fosc/(4*period);
//             printf("%s %4.1f  %s","Freq: ",Frequency,"Hz   ");
//             TimeClick=0;    //reset time reference
//         } //end if(TimeClick==1)
//         //Turn on the LED on tuner if frequency is below 440 Hz
//         if(Frequency > A4) TunerLED = ON;   else TunerLED = OFF;
//     } //end while(1)
// } //end main())
//courtesy of Prof. Hoover
void InitializeCCP1(void)
{
//Enable TIMER1 (See Fig. 6-1 TIMER1 Block Diagram in PIC16F887 Data Sheet)
    TMR1GE = 0;   TMR1ON = 1;
//Select internal clock whose frequency is Fosc/4, where Fosc = 4 MHz
    TMR1CS = 0;
//Set prescale to divide by 1 yielding a clock tick period of 1 microseconds
    T1CKPS1 = 0; T1CKPS0 = 0;
    //bit 5-4 T1CKPS<1:0>: Timer1 Input Clock Prescale Select bits
    //11 = 1:8 Prescale Value
    //10 = 1:4 Prescale Value
    //01 = 1:2 Prescale Value
    //00 = 1:1 Prescale Value
//Set CCP1 mode to toggle RC2 pin Compare
    CCP1M3 = 0; CCP1M2 = 0; CCP1M1 = 1; CCP1M0 = 0;
    TRISC2 = 0;                 //Make CCP1 pin an output.
    CCPR1 = TMR1 + CCP1Count;   //Schedule first CCP1 interrupt
    CCP1IF = 0;
    CCP1IE = 1;     //Enable Compare Interrupt from CCP1
    PEIE = 1;       //Enable all peripheral interrupts
}

void ServeCCP1(void) {
//    CCPR1 = TMR1+CCP1Count;   //set up next interrupt
    CCPR1 = CCPR1 + CCP1Count;  //more accurate to use CCPR1+CCP1Count
    CCP1IF = 0;         //clear CCP1 interrupt flag before returning.
//decrement by 1 each time one period of a note is played.
    Beats = Beats - 1;
}

void interrupt interrupt_handler(void)
{
    if (CCP1IF == 1) ServeCCP1(); //service CCP1 interrupt first
    if (T0IF == 1) ServeFlashLED(); //service Timer0 interrupt
    if (CCP2IF == 1) ServeCCP2(); //serve input capture CCP2 first
    // if (T0IF==1) ServeFlashLED();   //serve Timer0 overflow interrupt

}


//2
void putch(unsigned char byte) //putch() for printf()
{
    lcd_putch(byte);
}
//TimeLick is set each time LED is toggled by Timer0 interrupt to provide a time reference
extern unsigned char TimeClick; //define in Timer0FlashLED.c

//courtesy of Prof. Hoover
void InitializeCCP2(void)
{
    TMR1GE = 0;   TMR1ON = 1; //Enable TIMER1
    TMR1CS = 0;             //Select internal clock whose frequency is Fosc/4, where Fosc = 4 MHz
    T1CKPS1 = 0; T1CKPS0 = 0;   //Set prescale to divide by 1 yielding a clock tick period of 1 us
    //bit 5-4 T1CKPS<1:0>: Timer1 Input Clock Prescale Select bits
    //11 = 1:8 Prescale Value
    //10 = 1:4 Prescale Value
    //01 = 1:2 Prescale Value
    //00 = 1:1 Prescale Value
    TRISC1 = 1;     //Set CCP2 Pin as an input.
//Set CCP2 mode to enable Capture on every rising edge on CCP2 pin RC1
    CCP2M3 = 0; CCP2M2 = 1; CCP2M1 = 0; CCP2M0 = 1;
    CCP2IE = 1;     //Enable Capture Interrupt from CCP2
    CCP2IF = 0;     //Clear interrupt flag
    PEIE = 1;       //Enable all peripheral interrupts
}

void ServeCCP2(void) {
    static unsigned int old_value;  //local and permanent
    period = CCPR2;
    period = period - old_value;
    old_value = CCPR2;
    CCP2IF = 0;     //clear interrupt flag after it is serviced
}
