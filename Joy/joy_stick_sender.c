//Robot laser tag game
//James Guo
//Autmn 2016
//joy_stick_sender.c

//Sends the movement commands to the tank via the bluetooth dongle.
//robot tank recieves and responds accordingly

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

/* baudrate settings are defined in <asm/termbits.h>, which is
   included by <termios.h> */
#define BAUDRATE B115200   // Change as needed, keep B

/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyO1" //Beaglebone Black serial port

#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1


#include "Signals.h"
#include "joy_stick_lcd.h"

int main(){
	//Initialization of Buttons on joystick
	//active high joystick button = gpio 60
	//(button active low)

	// Creates the File pointers to create the gpio file system,
	// set the direction of the GPIO, and change the data stored there.
	FILE *sys, *dir;
	// Specifies the file that the pointer will be used for (w = //write)
	sys = fopen("/sys/class/gpio/export", "w");
   
	// int fseek(FILE *stream, long int offset, int whence)
	// 	sets the file position of the stream to the given offset.
	//		Whence has the following constants:
	//			SEEK_SET = Beginning of file
	//			SEEK_CUR = Current position of the file pointer
	//			SEEK_END = End of file
	//
	//		In this instance fseek is not necessarily needed.
	fseek(sys, 0, SEEK_SET);
	fprintf(sys, "%d", 60);
	fflush(sys);


	char routeB[50];
	sprintf(routeB, "/sys/class/gpio/gpio60/direction");
	dir = fopen(routeB, "w");
	fseek(dir, 0, SEEK_SET);
	fprintf(dir, "%s", "in");
	fflush(dir);
	fclose(dir);

	//Initialization of dx and dy adc for joystick
	
	FILE *dx, *dy, *play;
	play = fopen("/sys/devices/bone_capemgr.9/slots", "w");
    fseek(play, 0, SEEK_SET);
    fprintf(play, "%s", "cape-bone-iio");
    fflush(play);

	dx = fopen("/sys/devices/ocp.3/helper.15/AIN5", "r");
	dy = fopen("/sys/devices/ocp.3/helper.15/AIN6", "r");
	
	if (dx == NULL) {
		dx = fopen("/sys/devices/ocp.3/helper.17/AIN5", "r");
		dy = fopen("/sys/devices/ocp.3/helper.17/AIN6", "r");
	}

	char buffx [50];
	char buffy [50];
	char disx [50];
	char disy [50];
	int valx;
	int valy;

	//speed and direction states for the tank 
	int speed = 0;
	int direction = 0;


	//Initalize uart for bluetooth
	int fd, c, res;
   struct termios oldtio, newtio;
   char buf[255];
   // Load the pin configuration
   int ret = system("echo BB-UART1 > /sys/devices/bone_capemgr.9/slots");
   /* Open modem device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C. */
   fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
   if (fd < 0) { perror(MODEMDEVICE); exit(-1); }

   bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

   /* BAUDRATE: Set bps rate. You could also use cfsetispeed an[9d cfsetospeed.
    CRTSCTS : output hardware flow control (only used if the cable has
              all necessary lines. See sect. 7 of Serial-HOWTO)
    CS8     : 8n1 (8bit,no parity,1 stopbit)
    CLOCAL  : local connection, no modem contol
    CREAD   : enable receiving characters */
   newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

   /* IGNPAR  : ignore bytes with parity errors
    otherwise make device raw (no other input processing) */
   newtio.c_iflag = IGNPAR;

   /*  Raw output  */
   newtio.c_oflag = 0;

   /* ICANON  : enable canonical input
    disable all echo functionality, and don't send signals to calling program */
   newtio.c_lflag = ICANON;
   /* now clean the modem line and activate the settings for the port */
   tcflush(fd, TCIFLUSH);
   tcsetattr(fd,TCSANOW,&newtio);
   close(fd);

   //pointer to scan button presses
   FILE *button;
   int PushJ = 1;

   //main logic loop, actively sending movement signals to tank
	
	while(1){
		
		//open serial out port
		FILE *BL = fopen("/dev/ttyO1", "w");


		char buffer[2];
		//initialize button
		button = fopen("/sys/class/gpio/gpio60/value", "w+");

		fseek(button, SEEK_SET, 0);
		fread(buffer, 2, 2, button);
		PushJ = atoi(buffer);
		fclose(button);

		//PushJ=auto forward with max speed, break if any other actions performed on joystick 
		//(when butten is pressed, robot tank moves forward w/ max speed)
		if(PushJ == 0){
			while(PushJ == 0){
				char buffer[2];
				button = fopen("/sys/class/gpio/gpio60/value", "w+");

				fseek(button, SEEK_SET, 0);
				fread(buffer, 2, 2, button);
				PushJ = atoi(buffer);
				fclose(button);

				//sends forward max speed signal to tank
				fprintf(BL, "%d", FORWARD3);
				fprintf(BL, "\n");
				fflush(BL);
				usleep(10000);
			}

		}


		fseek(dx, 0,SEEK_SET);
		fread(buffx, 2, 2, dx);
		//printf("test");
		//d0 with the distance value I need
		//read from d0 and put it in int val0
		valx = atoi(buffx);
		
		fflush(dx);


		//determine direction of tank via x axis of joystick
		//direction: 0 = front, 1 = right, 2 = left
		//adjust accordingly if using different potentiometer joysticks
		if(valx > 800 && valx < 950){
			direction = 0;
		}else if(valx > 1600){
			direction = 1;
		}else if(valx < 130){
			direction = 2;
		}


		fseek(dy, 0,SEEK_SET);
		fread(buffy, 2, 2, dy);
		//printf("test");
		valy = atoi(buffy);
		//d0 with the distance value I need
		//read from d0 and put it in int val0
		fflush(dy);
		//printf("AINy : %s\n", buffy);
		//printf("valy: %d\n", valy);


		//determine spyeed according to y axis of joystick
		//speed: 0 = still, 1 = forward 1, 2 = forward 2, -1 = backwards
		if(valy > 850 && valy < 1000){
			speed = 0;
			
		}else if(valy > 2 && valy < 850){
			speed = 1;
		}else if(valy < 2){
			speed = 2;
		}else if(valy > 1000){
			speed = -1;
		}

		//nested logic loop to determine what direction/speed signal to send to tank
		//according to speed and direction
		if (abs(valy - 900) > abs(valx - 900)) {
			if (speed == 2) {
				/*shiftGear(3);
				moveForward();*/
				fprintf(BL, "%d", FORWARD1);
				fprintf(BL, "\n");
				fflush(BL);
			} else if (speed == 1) {
				/*shiftGear(1);
				moveForward();*/
				fprintf(BL, "%d", FORWARD3);
				fprintf(BL, "\n");

				fflush(BL);
			} else if (speed == -1) {
				/*moveBackward();*/
				fprintf(BL, "%d", BACKWARD);
				fprintf(BL, "\n");
				fflush(BL);
			} else {
				//shiftGear(0);
				fprintf(BL, "%d", STANDBY);
				fprintf(BL, "\n");
				fflush(BL);
			}
		} else {
			if (direction == 1) {
				//turnRight();
				fprintf(BL, "%d", RIGHT);
				fprintf(BL, "\n");
				fflush(BL);
			} else if (direction == 2) {
				//turnLeft();
				fprintf(BL, "%d", LEFT);
				fprintf(BL, "\n");
				fflush(BL);
			}
		}
		fclose(BL);
		usleep(10000);


	}

	return 0;
}
