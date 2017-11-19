#include "joy_stick_lcd.h"

//Initiallizes the LCD

int main() {
	// Creates the File pointers to create the gpio file system,
	// set the direction of the GPIO, and change the data stored there.

    //system("echo BB-UART4 > /sys/devices/bone_capemgr.9/slots");
    //system("echo BB-UART1 > /sys/devices/bone_capemgr.9/slots");
	FILE *sys, *dir, *file, *duty;
	/*duty = fopen("/sys/devices/ocp.3/pwm_test_P8_19.15/duty", "w");
	fprintf(duty, "%d", 9999);
	fflush(duty);*/
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


	// initialize LCD
	write10Bits(0,0,0,0,0,0,0,0,0,1,file);	// Clear Display

	write10Bits(0,0,0,0,1,1,1,0,-1,-1,file);	// turn on second line
	write10Bits(0,0,0,0,0,0,1,1,1,0,file);
	write10Bits(0,0,0,0,0,0,0,1,1,0,file);

	writeString("LCD INITIALIZED", file);
	return 0;
}