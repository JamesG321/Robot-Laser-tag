//Recevies hits from the tank and keeps track of HP and game logic

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

#define HPMAX 1001

/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyO4" //Beaglebone Black serial port

#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

#include "Signals.h"
#include "joy_stick_lcd.h"

int main() {
  //lcd ini
  FILE *file;
	write10Bits(0,0,0,0,0,0,0,0,0,1,file);	// Clear Display

	write10Bits(0,0,0,0,1,1,1,0,-1,-1,file);	// turn on second line
	write10Bits(0,0,0,0,0,0,1,1,1,0,file);
	write10Bits(0,0,0,0,0,0,0,1,1,0,file);


  //A = gpio 20, B = gpio 112(button active low), 
  //vibration = gpio 115
  //initialze gpios

  // Creates the File pointers to create the gpio file system,
  // set the direction of the GPIO, and change the data stored there.
  FILE *sys, *dir;
  // Specifies the file that the pointer will be used for (w = //write)
  sys = fopen("/sys/class/gpio/export", "w");
   
  // int fseek(FILE *stream, long int offset, int whence)
  //  sets the file position of the stream to the given offset.
  //    Whence has the following constants:
  //      SEEK_SET = Beginning of file
  //      SEEK_CUR = Current position of the file pointer
  //      SEEK_END = End of file
  //
  //    In this instance fseek is not necessarily needed.

  fseek(sys, 0, SEEK_SET);
  fprintf(sys, "%d", 20);
  fflush(sys);

  fseek(sys, 0, SEEK_SET);
  fprintf(sys, "%d", 112);
  fflush(sys);

  fseek(sys, 0, SEEK_SET);
  fprintf(sys, "%d", 115);
  fflush(sys);

  char routeA[50];
  sprintf(routeA, "/sys/class/gpio/gpio112/direction");
  dir = fopen(routeA, "w");
  fseek(dir, 0, SEEK_SET);
  fprintf(dir, "%s", "in");
  fflush(dir);
  fclose(dir);

  char routeJ[50];
  sprintf(routeJ, "/sys/class/gpio/gpio20/direction");
  dir = fopen(routeJ, "w");
  fseek(dir, 0, SEEK_SET);
  fprintf(dir, "%s", "in");
  fflush(dir);
  fclose(dir);

  char routeV[50];
  sprintf(routeV, "/sys/class/gpio/gpio115/direction");
  dir = fopen(routeV, "w");
  fseek(dir, 0, SEEK_SET);
  fprintf(dir, "%s", "out");
  fflush(dir);
  fclose(dir);


  FILE *button;
  int PushB = 0;
  int PushA = 0;



	int fd, c, res;
   struct termios oldtio, newtio;
   char buf[255];
   // Load the pin configuration
   int ret = system("echo BB-UART4 > /sys/devices/bone_capemgr.9/slots");
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
   newtio.c_iflag = IGNPAR | BRKINT;

   /*  Raw output  */
   newtio.c_oflag = OPOST;

   /* ICANON  : enable canonical input
    disable all echo functionality, and don't send signals to calling program */
   newtio.c_lflag = ICANON;
   /* now clean the modem line and activate the settings for the port */
   tcflush(fd, TCIFLUSH);
   tcsetattr(fd,TCSANOW,&newtio);

  //Initialize display for game
  int HP = HPMAX;
	writeString("HP: 1001", file);

  FILE *vib;

	while (1) {
    //scan value of button B (unused in this application but customizeable for controller)
/*    char buffer[2];
    button = fopen("/sys/class/gpio/gpio112/value", "w+");
    fseek(button, SEEK_SET, 0);
    fread(buffer, 2, 2, button);
    PushB = atoi(buffer);
    fclose(button);

    //scan value of A (unused in this application but customizeable for controller)
    button = fopen("/sys/class/gpio/gpio20/value", "w+");
    fseek(button, SEEK_SET, 0);
    fread(buffer, 2, 2, button);
    PushA = atoi(buffer);
    fclose(button);
*/

		res = read(fd, buf, 255);
		buf[res] = 0;             
    //set end of string, so we can printf 

		int hit = atoi(buf);
		printf("%d\n", HP);

    

		if (hit == HIT) {
			HP--;
			clear(file);
			char hp[10];
			sprintf(hp, "HP: %d", HP);
			writeString(hp, file);
      changeGPIOValue(1, 115, vib);


      
			if (HP == 0) {
				clear(file);
				writeString("GAME OVER!", file);
				exit(1);
			}
		} 

		usleep(80000);
    changeGPIOValue(0, 115, vib);

 
    
	} 
}

