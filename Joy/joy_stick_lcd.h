//Robot laser tag game
//James Guo
//Autmn 2016
//joy_stick_lcd.h

//header file for defining methods for LCD screen

#ifndef _JOY_STICK_LCD_H_
#define _JOY_STICK_LCD_H_

#include <linux/stat.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>       // for usleep
#include <string.h>

//GPIO pins #s corresponding to lcd screen
//Used for specefic layout from project
//should be changed depending on acutal pin layout.
//See lcd manual from documentation for more details of the interface.
#define RS 66
#define RW 67
#define	E 68
#define	DB0 69
#define DB1 44
#define DB2 45
#define DB3 26
#define DB4 46
#define DB5 47
#define DB6 65
#define DB7 27

#define GPIO 11

void enableRead(FILE *file);

// change the value of a specific gpio_num with val;
// return 	0 for success,
//			-1 for error.
int changeGPIOValue(int val, int gpio_num, FILE *file);

// Write bits into RS, R/W and DB0 to DB7 of a ; does not write if the input is -1;
// return	0 for success,
//			-1 for error.
int write10Bits(int NewRS, int NewRW, int NewDB7, int NewDB6, int NewDB5,
				int NewDB4, int NewDB3, int NewDB2, int NewDB1, int NewDB0, FILE *file);

//turn to write first line mode
void firstLine(FILE *file);

//turn to write second line mode
void secondLine(FILE *file);

//clear screen
void clear(FILE *file);

//turn off screen
void turnOff(FILE *file);

//write one character to lcd
void writeChar(char ch, FILE *file);

//write string to lcd
void writeString(const char* bufSource, FILE *file);


#endif