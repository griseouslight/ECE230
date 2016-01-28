    /*
 * File:   main.c
 * Author: Talon
 *
 * Created on January 18, 2016, 8:21 PM
 */


#include <xc.h>



//1.1 Complete the init function for CCP1 and Timer1

#define HighPulseWidth 600
#define LowPulseWidth 1500
void Timer_CCP_init(void) {
    TMR1GE = 0; TMR1ON = 1; // Enable TIMER1
// Select internal clock whose frequency is Fosc/4, Fosc = 8 MHz
    TMR1CS = 0;
    T1CKPS1 = 0; T1CKPS0 = 0;
    CCP1M3 = 0; CCP1M2 = 0; CCP1M1 = 1; CCP1M0 = 0; //toggle pin
    TRISC2 = 0; // Make CCP1 pin an output.
    RC2=1; CCPR1=TMR1+HighPulseWidth; //to start first interrupt on high pulse
    CCP1IF = 0; // Clear the CCP1IF interrupt flag
    CCP1IE = 1; // Enable Compare Interrupt from CCP1
    PEIE = 1; // Enable all peripheral interrupts
    GIE = 1; // Globally Enable all interrupts
}
//1.2 Complete the interrupt handler function. You may add global variables if needed.
void interrupt interrupt_handler(void)
{
    if (CCP1IF == 1) {
        CCPR1 = (RC2 == 1) ? HighPulseWidth : LowPulseWidth;
        
        CCP1IF = 0;
    }
} 


void main(void) {
    Timer_CCP_init();
    while(1);
}