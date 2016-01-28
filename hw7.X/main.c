/*
 * File:   main.c
 * Author: Talon
 *
 * Created on January 24, 2016, 9:29 PM
 */


#include <xc.h>
void init(void);
void interrupt interrupt_handler(void);


void main(void) {
    init();
    
    for(;;);
    return;
}


void init(void){
    ANSEL = 0;
    ANSELH = 0; //everything is digital
    nRBPU = 0;
    
    TRISD = 0; //all (including needed bits) are 0;
    TRISB4 = 1; //RB4 is input
    
    IOCB4 = 1;
    RBIF = 0; RBIE = 1; GIE = 1;
//    INTCON3 = 1;
    
}


void interrupt interrupt_hander(void) {
    if (RBIF == 1) {
        if (RB4 == 1) PORTD++;
        RBIF = 0;
    }
    
}