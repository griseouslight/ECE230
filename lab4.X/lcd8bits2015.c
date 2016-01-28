// Created by Hi-Tech Software Modified by Jianjian Song to work with PIC16F887, August 13, 2009
// Modified by Keith Hoover for easier use in ECE230, July 15, 2011
// Modified by Chris Miller for easier use in ECE230, September, 2014
/*
 *	"lcd8bits2015.c" LCD Display Panel Driver Routines
 *  Reference:  Refer to the Optrex LCD User Manual for all details and command formats
 *              used in these routines "Optrex LCD Manual Dmcman_full-user manual.pdf" 
 *
 *	All necessary LCD and delay functions have been bundled into this file (lcd8bit.c),
 *  and the necessary function prototype statements are bundled into the 
 *  companion header file, lcd4bit.h.  Therefore by including lcd4bit.c into 
 *  a project along with a separate "main.c" program that calls one or more of these
 *  the LCD panel display routines and contains an "#include "lcd8bit.h" header file,
 *  you need not cut and paste all of these routines into the main program file itself.
 *
 *	This code will interface to a wide variety of B/W LCD Display Controllers
 *	like the Hitachi HD44780. It uses the 8-bit transfer mode, and it assumes that the
 *      pins of PORT D (RD7:0) are connected to the LCD panel, with
 *	the hardware connected as follows, assuming the standard 16 pin LCD pinout:
 
 *    GND to VSS Pin of LCD Pin 1
 *    +5V to VDD Pin of LCD Pin 2
 *     ~  to V0 Pin of LCD Pin 3
 *    GND to R/W* Pin of LCD Pin 5 (This means that the software can only write to the LCD panel, and never
 *                                  read from it.)
 *	  RD0:7 are connected to the LCD data bits 0-7 LCD Pins 7-14
 *	  RC4 is connected to the LCD RS input (register select) LCD Pin 4
 *  	  RC5 is connected to the LCD EN bit (enable)  LCD Pin 6
 *	
 *	The available routines in this file are:
 *
 *    1.  lcd_init( ) 
 *        Always call lcd_init() first, which follows the manufacturer's
 *        directions for initializing the LCD display panel into 8-bit transfer mode.
 *        Then you may call any of the other routines, as needed.  Note that this
 *        initialization routine also makes RD7:0 all outputs, as required to drive
 *		  the LCD panel connected to RD7:0.  It also selects 8 MHz internal clock.
 *
 *    2.  lcd_clear( )
 *        Clears LCD display and homes the cursor
 *
 *	  3.  lcd_puts(const char s*)
 *        writes a constant character string to the lcd panel
 *
 *    4.  lcd_putch(char s)
 *        writes a single character to lcd panel
 *
 *    5.  lcd_goto(unsigned char pos)
 *        Moves cursor to desired position.  For 16 x 2 LCD display panel, 
 *        the columns of Row 1 are 0x00....0x10
 *        the columns of Row 2 are 0x40....0x50
 *
 *	  6.  DelayMs(unsigned int NrMs)
 *		  Delays for NrMs milliseconds.
 *
 */

#include	<xc.h>

void DelayMs(unsigned int);
#define CMD_MODE        0           //0 for command mode
#define DTA_MODE        1           //1 for data mode
#define LCD_RS          RC4         //pin 4 -LCD command/data 
#define LCD_EN          RC5         //pin 6 -LCD Enable (clocks data into LCD on falling edge) 
#define LCD_DATA        PORTD       //rename PORTD 	-LCD Data pins (D0 - D7)
#define LCDCMD_ClearDisplay     0x01    //clear display: clear, move cursor home
#define LCDCMD_EMS              0x06    //entry mode set: auto increment cursor after each char sent
#define LCDCMD_DisplaySettings  0x0C    //display ON/OFF control: display on, cursor off, blink off
#define LCDCMD_FunctionSet      0x38    //function set: 8-bit mode, 2 lines, 5x7 dots

#define	LCD_STROBE()	((LCD_EN = 1),(LCD_EN=0))   //This macro definition pulses (strobes) the E line
//making it rise and then fall.  This falling edge
//writes data on LCD Panel pins DB7:4 into the LCD Panel.

void DelayMs(unsigned int nrms) {
    unsigned int i, j;
    for (j = 0; j < nrms; j++)
        for (i = 0; i < 20; i++);
}

/*
 * lcd_write function ---writes a byte to the LCD in 8-bit mode
 * Note that the "mode" argument is set to either CMD_MODE (=0) or DTA_MODE (=1), so that the
 * LCD panel knows whether an instruction byte is being written to it or an ASCII code is being written to it
 * that is to be displayed on the panel.
 */ 
void lcd_write(unsigned char mode, unsigned char CmdChar) {
    LCD_RS = mode;
    DelayMs(10);
    LCD_DATA = CmdChar;
    LCD_STROBE(); // Write 8 bits of data on D7-0
}

/*
 * 	Clear and home the LCD
 */
void lcd_clear(void) {
    lcd_write(CMD_MODE, LCDCMD_ClearDisplay);
    DelayMs(2);
}

/* write a string of chars to the LCD */
void lcd_puts(const char *string) {
    while (*string != 0) // Last character in a C-language string is alway "0" (ASCII NULL character)
        lcd_write(DTA_MODE, *string++);
}

/* write one character to the LCD */
void lcd_putch(char character) {
    lcd_write(DTA_MODE, character);
}

/*
 * Moves cursor to desired position.  
 * For 16 x 2 LCD display panel, 
 *     the columns of Row 1 are 0x00....0x10
 *     the columns of Row 2 are 0x40....0x50
 */
void lcd_goto(unsigned char position) {
    lcd_write(CMD_MODE, 0x80 | position); // The "cursor move" command is indicated by MSB=1 (0x80)
    // followed by the panel position address (0x00- 0x7F)
}

/*
 * Initialize the LCD - put into 8 bit mode
 * assume 4 MHz clock
 */
void lcd_init() //See Section 2.2.2.2 of the Optrex LCD DMCman User Manual
{
    TRISD = 0b00000000; //Make PORTD (D7-0) all output
    TRISC4 = 0; //Make RC4 (RS) output
    TRISC5 = 0; //Make RC5 (EN) output
    LCD_RS = CMD_MODE;
    LCD_EN = 0;
    DelayMs(15); // wait 15mSec after power applied,
    lcd_write(CMD_MODE, LCDCMD_FunctionSet); // function set: 8-bit mode, 2 lines, 5x7 dots
    lcd_write(CMD_MODE, LCDCMD_DisplaySettings); // display ON/OFF control: display on, cursor off, blink off
    lcd_clear(); // Clear screen
    lcd_write(CMD_MODE, LCDCMD_EMS); // Set entry Mode
}

void Display(unsigned char moles, unsigned int score) {
    unsigned char digits[4];
    unsigned char i;
    lcd_clear();
    if(score > 2500)
        for (i = 0; i < 4; i++) {
            digits[i] = 'X' - 0x30;
        }
    else
        for (i = 0; i < 4; i++) {
            digits[3 - i] = (score % 10);
            score = score / 10;
        }
    lcd_goto(0); // select first line
    lcd_puts("Moles Hit: ");
    if(moles <= 5)
        lcd_putch(moles + 0x30);
    else
        lcd_putch('X');
    lcd_goto(0x41); // Select second line and 3rd column
    lcd_puts("Score: ");
    i = 0;
    while(digits[i] == 0) {
        lcd_putch(' ');
        i++;
    }
    for (i = 0; ((digits[i] == 0) && (i < 3)); i++)
        lcd_putch(' ');
    for ( ; i < 4; i++)
        lcd_putch(digits[i] + 0x30);

}
