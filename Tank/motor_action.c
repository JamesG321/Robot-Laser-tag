//Robot laser tag game
//James Guo
//Autmn 2016
//motor_action.c

//functions for controlling the motors. used H-bridge as motor driver for the tank.
//tank has 2 motors, A on left and B on right.
//adjust code/wiring accordingly to use on different chasis.
#include "motor_action.h"

#include <unistd.h>

//"turns" (rotates) the motor by sending corresponding signal to H-bridge.
static void turn(int i, int gpio_num) {
	int fd;
	char path[50];
	sprintf(path, "/sys/class/gpio/gpio%d/value", gpio_num);
	fd = open(path, O_RDWR);
	char num[2];
	sprintf(num, "%d", i);
	write(fd,num,2);
	close(fd);
}

//sends signal so motorA turns forward
static void AFoward() {
	turn(0, AIN1);
	//usleep(500);
	turn(1, AIN2);
	//usleep(500);
}

//sends signal so motorA turns backward
static void ABackward() {
	turn(1, AIN1);
	//usleep(500);
	turn(0, AIN2);
	//usleep(500);
}

//sends signal so motorB turns forward
static void BFoward() {
	turn(1, BIN1);
	//usleep(500);
	turn(0, BIN2);
	//usleep(500);
}

//sends signal so motorB turns backward
static void BBackward() {
	turn(0, BIN1);
	//usleep(500);
	turn(1, BIN2);
	//usleep(500);
}

//sends signal so motorA stops
static void AStop() {
	turn(0, AIN1);
	//usleep(500);
	turn(0, AIN2);
	//usleep(500);
}
//sends signal so motorB stops
static void BStop() {
	turn(0, BIN1);
	//usleep(500);
	turn(0, BIN2);
	//usleep(500);
}
//sends signal so motors stand by
static void standBy() {
	//printf("standBy\n");
	turn(0, STDBY);
	//usleep(500);
	AStop();
	BStop();
}

//sends signal so tank "moves" in no direction (stays still)
static void move() {
	turn(1, STDBY);
	//usleep(500);
}
//sends signal so tank moves forward
static void moveForward() {
	AFoward();
	BFoward();
}

//sends signal so tank moves backwards
static void moveBackward() {
	ABackward();
	BBackward();
}

//sends signal so tank turns left
static void turnLeft() {
	AFoward();
	BBackward();
}
//sends signal so tank turns right
static void turnRight() {
	BFoward();
	ABackward();
}
//changes the speed of motors, "shifting gears"
static void shiftGear(int number) {
	//
	char num[100];
	sprintf(num, "%d", Gears[number]);
	int fd1;
	fd1 = open("/sys/devices/ocp.3/pwm_test_P9_14.15/duty", O_RDWR);
	write(fd1,num,2);

	int fd2;
	fd2 = open("/sys/devices/ocp.3/pwm_test_P9_16.16/duty", O_RDWR);
	write(fd2,num,2);
	close(fd1);
	close(fd2);
}