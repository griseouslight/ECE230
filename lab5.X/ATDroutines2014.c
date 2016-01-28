//Project : ATDroutines2014Solution.c
//Date created   : January 2015, Solution for Lab #4 ECE331 Winter 2014-15
//Revised:  December 2014: separate ATDInitialization from ATDchannelSet
///Date revised: 
//Author  : jianjian song
//Company : rose-hulman institute of technology
//Purpose : ATD routines for PIC16F887

#include <xc.h>

void DelayTime(unsigned int count) {
	unsigned int i, j;
	for(i=0;i<count;i++) for(j=0;j<count;j++) {};
}


void ATDInitialization(void) {
//initialize the analog channel
//right justified;	FRS,	GND and VDD

	ADCS1 = 1;
	ADCS0 = 1;	//FRS 2-6 useconds conversion speed
	ADCON0bits.ADON = 1;	//ADC is enabled
	ADFM = 1;	//right justified
	VCFG1 = 0;	// Vss = GND
	VCFG0 = 0;	// VDD or VCC
}

#define ChannelSetDelay 2
void ATDsetChannel(unsigned char channel) {
//set analog channel
//configure input to be analog first before setting channel.
//Otherwise, the configuration may not work.
	switch(channel) {
		case 0: {	//on pin RA0
			TRISA0=1; ANS0=1; ADCON0bits.CHS=0b0000;
                        DelayTime(ChannelSetDelay);
			break; 
		}
		case 1: {	//RA1
                    TRISA1=1; ANS1=1;
                    ADCON0bits.CHS3=0; ADCON0bits.CHS2=0;
                    ADCON0bits.CHS1=0; ADCON0bits.CHS0=1;
                    DelayTime(ChannelSetDelay);
			break; 
		}
		case 2: {	//RA2
                        TRISA2=1; ANS2=1;
			CHS3=0; CHS2=0; CHS1=1; CHS0=0;
                        DelayTime(ChannelSetDelay);
			break;  }
		case 3: { //RA3
            TRISA3=1; ANS3=1;
                          CHS3=0; CHS2=0; CHS1=1; CHS0=1;
                          DelayTime(ChannelSetDelay); break;  }
		case 4: { TRISA5=1; ANS4=1;
                          CHS3=0; CHS2=1; CHS1=0; CHS0=0;
                          DelayTime(ChannelSetDelay); break;  }
                case 5: { TRISE0=1; ANS5=1;
                          CHS3=0; CHS2=1; CHS1=0; CHS0=1;
                          DelayTime(ChannelSetDelay); break;  }
		case 6: { TRISE1=1; ANS6=1;
                          CHS3=0; CHS2=1; CHS1=1; CHS0=0;
                          DelayTime(ChannelSetDelay); break;  }
                case 7: { TRISE2=1; ANS7=1;
                          CHS3=0; CHS2=1; CHS1=1; CHS0=1;
                          DelayTime(ChannelSetDelay); break;  }
                case 8: { TRISB2=1; ANS8=1;
                          CHS3=1; CHS2=0; CHS1=0; CHS0=0;
                          DelayTime(ChannelSetDelay); break;  }
                case 9: { TRISB3=1; ANS9=1;
                          CHS3=1; CHS2=0; CHS1=0; CHS0=1;
                          DelayTime(ChannelSetDelay); break;  }
                case 10: { TRISB1=1; ANS10=1;
                          CHS3=1; CHS2=0; CHS1=1; CHS0=0;
                          DelayTime(ChannelSetDelay); break;  }
		default: {	//RA0
                        TRISA0=1; ANS0=1;
			CHS3=0; CHS2=0; CHS1=0; CHS0=0; break; 
		}	
	}
}	//end ATDsetChannel()

void ATDgetVoltage(unsigned int *Voltage) {
//Convert analog voltage from default channel
//Return 10-bit digital voltage in *Voltage
	ADCON0bits.GO_nDONE = 1;
//wait until conversion is over
	while(ADCON0bits.GO_nDONE==1) {continue;};
	*Voltage = (unsigned int) ADRESH;
	*Voltage = (*Voltage)<<8;
	*Voltage = *Voltage+(unsigned int) ADRESL;
}	//end ATDgetVoltage()
