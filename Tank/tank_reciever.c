//Robot laser tag game
//James Guo
//Autmn 2016
//tank_reciever.c

//recevies movement commands from tank via bluetooth dongle
//used SparkFun Bluetooth Modem - BlueSMiRF

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
#define MODEMDEVICE "/dev/ttyO4" //Beaglebone Black serial port

#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

#include "Signals.h"
#include "motor_action.h"
#include "motor_action.c"

int main(){



   FILE *sys, *dir;
   int GPIOs[5] = {AIN1, AIN2, BIN1, BIN2, STDBY};
   // Specifies the file that the pointer will be used for (w = //write)
   sys = fopen("/sys/class/gpio/export", "w");
   //error detection for file ptr
   /*if (sys == NULL) {
     printf("errno: %d\n", errno);
     exit(0);
   }*/
   // export all GPIOs, and change their directions to "out"
   fseek(sys, 0, SEEK_SET);

   int i;
   for (i = 0; i < 5; i++) {
     fprintf(sys, "%d", GPIOs[i]);   // echo Number > export
     fflush(sys);    // save
     char route[50];
     sprintf(route, "/sys/class/gpio/gpio%d/direction", GPIOs[i]);   // route for each gpio numbers
     dir = fopen(route, "w");    // gpio/direction w: write access
     fseek(dir, 0, SEEK_SET);    // put the cursor to the head of the file
     fprintf(dir, "%s", "out");  // echo out > direction
     fflush(dir);    // save
   }

   FILE *play;
   //intialize pwn pins
   play = fopen("/sys/devices/bone_capemgr.9/slots", "w");
   fseek(play, 0, SEEK_SET);
   fprintf(play, "%s", "am33xx_pwm");
   fflush(play);
   fprintf(play, "%s", "bone_pwm_P9_14");
   fflush(play);

   fprintf(play, "%s", "bone_pwm_P9_16");
   fflush(play);

   play = fopen("/sys/devices/bone_capemgr.9/slots", "w");
   fseek(play, 0, SEEK_SET);
   fprintf(play, "%s", "cape-bone-iio");
   fflush(play);

   shiftGear(0);


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
   newtio.c_iflag = IGNPAR;

   /*  Raw output  */
   newtio.c_oflag = 0;

   /* ICANON  : enable canonical input
    disable all echo functionality, and don't send signals to calling program */
   newtio.c_lflag = ICANON;
   /* now clean the modem line and activate the settings for the port */
   tcflush(fd, TCIFLUSH);
   tcsetattr(fd,TCSANOW,&newtio);
   // NMEA command to ouput all sentences
   // Note that this code & format values in manual are hexadecimal

   int status = -1;
   //initialize status to dummy value
   while(1){
      res = read(fd, buf, 255);
      buf[res] = 0;             /* set end of string, so we can use it */
      //printf("%s", buf, res);

      //reads movement sent to serial port
      int movement = atoi(buf);
      //nested decesion tree to determine what gear and direction/speed the tank will move
      if (movement != status) {
        
        if (movement == FORWARD3) {
          status = FORWARD3;
           shiftGear(3);
            move();
           moveForward();
        } else if (movement == FORWARD1) {
          status = FORWARD1;
           shiftGear(1);
            move();
           moveForward();
        } else if (movement == BACKWARD) {
          status = BACKWARD;
           shiftGear(1);
            move();
           moveBackward();
        } else if (movement == STANDBY) {
          status = STANDBY;
           standBy();
        } else if (movement == LEFT) {
          status = LEFT;
           shiftGear(5);
            move();
           turnLeft();
           //printf("%s\n", "wtf");
        } else if (movement == RIGHT) {
          status = RIGHT;
           shiftGear(5);
            move();
           turnRight();
        }
      }
   }
   //return 0 if loop exits (bug encountered)
   return 0;
}
