#ECE 230
## Lab 4


* Displays measured frequency on RC1 pin to LCD in tenths of Hertz
	*  Accurately detect frequencies between 50 to 5000 Hz
	*  Display 0.0 Hz if no signal detected on RC1
	*  Do not display output frequency of RC2 on LCD
* Generate square-wave on RC2 to play note on speaker when SW2/3/4 is pressed
	*  Play notes A3, B3, and C4, respectively
* When SW1 is pressed, buttons SW2/3/4 will generate note at next higher octave
	*  Play notes A4, B4, and C5, respectively
* If SW2 and SW3 and SW4 is not pressed, no signal is generated on RC2 (no tone)
* LCD is readable and refresh rate is 4 Hz
* Tone generation, frequency measurement, and LCD display all function
simultaneously, and none effect the functionality of another 