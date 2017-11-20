//Robot laser tag game
//James Guo
//Autmn 2016
//joy_stick_lcd_main.c

//main .c file for interface between lcd and beaglebone board.
//pin I/O can be changed via joy_stick_lcd.h
//not actually used in this application
//used for reference.
//See lcd manual from documentation for more details of the interface.

#include "joy_stick_lcd.h"



int main() {
	// Creates the File pointers to create the gpio file system,
	// set the direction of the GPIO, and change the data stored there.

	FILE *sys, *dir, *file, *duty;
	// an array for storing all GPIO_PIN numbers being used.
	int GPIOs[GPIO] = {RS, RW, E, DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7};

	// Specifies the file that the pointer will be used for (w = //write)
	sys = fopen("/sys/class/gpio/export", "w");
   	
	// export all GPIOs, and change their directions to "out"
	fseek(sys, 0, SEEK_SET);

	int i;
	for (i = 0; i < GPIO; i++) {
		fprintf(sys, "%d", GPIOs[i]);	// echo Number > export
		fflush(sys);	// save
		char route[50];
		sprintf(route, "/sys/class/gpio/gpio%d/direction", GPIOs[i]);	// route for each gpio numbers
		dir = fopen(route, "w");	// gpio/direction w: write access
		fseek(dir, 0, SEEK_SET);	// put the cursor to the head of the file
		fprintf(dir, "%s", "out");	// echo out > direction
		fflush(dir);	// save
	}



	clear(file);	// Clear Display
	// initialize LCD
	write10Bits(0,0,0,0,1,1,1,0,-1,-1,file);	// turn on second line
	write10Bits(0,0,0,0,0,0,1,1,1,0,file);
	write10Bits(0,0,0,0,0,0,0,1,1,0,file);

	writeString("LCD INITIALIZED", file);

	//lcd operations should go here.

	return 0;
}