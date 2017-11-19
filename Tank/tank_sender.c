//Sends hit detection
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

/* baudrate settings are defined in <asm/termbits.h>, which is
   included by <termios.h> */
#define BAUDRATE B115200   // Change as needed, keep B

/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyO1" //Beaglebone Black serial port

#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

#include "Signals.h"
#include "motor_action.h"
#include "motor_action.c"

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

int main() {
	FILE *dx, *dy, *play;
	
	play = fopen("/sys/devices/bone_capemgr.9/slots", "w");
        fseek(play, 0, SEEK_SET);
        fprintf(play, "%s", "cape-bone-iio");
        fflush(play);

	dx = fopen("/sys/devices/ocp.3/helper.15/AIN5", "r");
	dy = fopen("/sys/devices/ocp.3/helper.15/AIN0", "r");

	if (dx == NULL) {

		dx = fopen("/sys/devices/ocp.3/helper.17/AIN5", "r");
		dy = fopen("/sys/devices/ocp.3/helper.17/AIN0", "r");
	}

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

    FILE *sys = fopen("/sys/class/gpio/export", "w");
    FILE *dir, *file;
   	if (sys == NULL) {
   		//printf("errno: %d\n", errno);
   		exit(0);
   	}
	// export all GPIOs, and change their directions to "out"
	fseek(sys, 0, SEEK_SET);

	fprintf(sys, "%d", 48);	// echo Number > export
	fflush(sys);	// save
	char route[50];
	sprintf(route, "/sys/class/gpio/gpio%d/direction", 48);	// route for each gpio numbers
	dir = fopen(route, "w");	// gpio/direction w: write access
	fseek(dir, 0, SEEK_SET);	// put the cursor to the head of the file
	fprintf(dir, "%s", "out");	// echo out > direction
	fflush(dir);	// save
	int cnt = 0;
	int temp = 0;

	FILE *BL = fopen("/dev/ttyO1", "w");
	while (1) {
		char buffx [50];
		char buffy [50];
		char disx [50];
		char disy [50];
		int valx;
		int valy;
		fseek(dx, 0,SEEK_SET);
	        fread(buffx, 2, 2, dx);
		//printf("test");
		//d0 with the distance value I need
		//read from d0 and put it in int val0
		valx = atoi(buffx);
		fflush(dx);


		fseek(dy, 0,SEEK_SET);
		fread(buffy, 2, 2, dy);
		//printf("test");
		valy = atoi(buffy);
		//d0 with the distance value I need
		//read from d0 and put it in int val0
		fflush(dy);

		//printf("front: %d\n", valx);
		//printf("back: %d\n", valy);
		//printf("valy: %d, valx: %d\n", valy, valx);
		if (valy > 50 || valx > 50) {
			//printf("valy: %d, valx: %d\n", valy, valx);
			if (temp == 1) {
				cnt++;
				if (cnt == 1) {
					changeGPIOValue(1, 48, file);
					fprintf(BL, "%d", HIT);
					fprintf(BL, "\n");
					fflush(BL);
					printf("valx: %d valy: %d sent signal: %d\n", valx, valy, HIT);
					cnt = 0;
					temp = 0;
					changeGPIOValue(0, 48, file);
				}
			} else {
				cnt = 0;
			}
			temp = 1;
		} else {
			temp = 0;
			cnt = 0;
		}
		usleep(10000);
	}
	return 0;
}
