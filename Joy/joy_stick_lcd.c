//Robot laser tag game
//James Guo
//Autmn 2016
//joy_stick_lcd.c

//methods to interface between lcd and beaglebone board.
//pin I/O can be changed via joy_stick_lcd.h
//See lcd manual from documentation for more details of the interface.


#include "joy_stick_lcd.h"

#include <time.h>

//Module for LCD control

//change 
void enableRead(FILE *file) {
	changeGPIOValue(1, E, file);
	usleep(4000);
	changeGPIOValue(0, E, file);
}

// change the value of a specific gpio_num with val;
int changeGPIOValue(int val, int gpio_num, FILE *file) {
	if (val == -1) {
		val = 0;
	}
	char path[50];
	sprintf(path, "/sys/class/gpio/gpio%d/value", gpio_num);
	file = fopen(path, "w");
	fseek(file, 0, SEEK_SET);
	fprintf(file, "%d", val);
	fflush(file);
	fclose(file);
	return 0;
}

// Write bits into RS, R/W and DB0 to DB7 of a ; does not write if the input is -1;
//
int write10Bits(int NewRS, int NewRW, int NewDB7, int NewDB6, int NewDB5,
				int NewDB4, int NewDB3, int NewDB2, int NewDB1, int NewDB0, FILE *file) {
	changeGPIOValue(NewRS, RS, file);
	changeGPIOValue(NewRW, RW, file);
	changeGPIOValue(NewDB7, DB7, file);
	changeGPIOValue(NewDB6, DB6, file);
	changeGPIOValue(NewDB5, DB5, file);
	changeGPIOValue(NewDB4, DB4, file);
	changeGPIOValue(NewDB3, DB3, file);
	changeGPIOValue(NewDB2, DB2, file);
	changeGPIOValue(NewDB1, DB1, file);
	changeGPIOValue(NewDB0, DB0, file);
	enableRead(file);
	return 0;
}
//turn to write first line mode
void firstLine(FILE *file) {
	write10Bits(0,0,1,0,0,0,0,0,0,0,file);
}

//turn to write second line mode
void secondLine(FILE *file) {
	write10Bits(0,0,1,1,0,0,0,0,0,0,file);
}

//sends sequence to clear the characters on the LCD screen
void clear(FILE *file) {
	write10Bits(0,0,0,0,0,0,0,0,0,1,file);
}


//sends sequence to turn lcd display OFF
void turnOff(FILE *file) {
	write10Bits(0,0,0,0,0,0,1,0,0,0,file);	
}

//sends sequence to write an individual char to the lcd screen
void writeChar(char ch, FILE *file) {
	int bits[8];
	int i = 0;
	while (i <= 7) {
		bits[i] = ch & 1;
		ch = ch >> 1;
		i++;
	}
	write10Bits(1,0,bits[7],bits[6],bits[5],bits[4],bits[3],bits[2],bits[1],bits[0],file);
}

//Writes a given string to the lcd string 
//uses writeChar() 
void writeString(const char* bufSource, FILE *file) {
	int i;
	for (i = 0; i < strlen(bufSource); i++) {
		//printf("i: %d\n", i);
		writeChar((char) bufSource[i], file);
	}
}