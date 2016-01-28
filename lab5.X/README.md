#ECE 230
## Lab 5

This program's purpose was to turn a servo with an Infared Sensor on it. Project Requirements:

* Circuit initially in Manual Mode
* Manual Mode
	* Potentiometer controls position of servo
	* Servo turns full 180°
	* Voltage from pot circuit displayed on LCD as: x.xx Volts
	* LCD display updated at least 4Hz
	* The system clock is generated from the 8MHz external crystal.
* Auto Mode
	* During scan LCD displays: Scanning . . .
	* Servo performs complete 180° scan
	* Voltage from phototransistor circuit displayed on LCD as: x.xx Volts
	* Frequent PT readings performed during scan for high resolution
	* Following scan, servo returns to position of highest light intensity reading
	* Upon completion, LCD displays: Scan Complete
* Pressing Mode button (SW1) toggles between Manual and Auto modes
* 4-bit LCD
	* .LCD circuit uses only 4-bit data lines. 