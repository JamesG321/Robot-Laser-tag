//Module for controlling the motors
#include "motor_action.h"

#include <unistd.h>

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

static void AFoward() {
	turn(0, AIN1);
	//usleep(500);
	turn(1, AIN2);
	//usleep(500);
}

static void ABackward() {
	turn(1, AIN1);
	//usleep(500);
	turn(0, AIN2);
	//usleep(500);
}

static void BFoward() {
	turn(1, BIN1);
	//usleep(500);
	turn(0, BIN2);
	//usleep(500);
}

static void BBackward() {
	turn(0, BIN1);
	//usleep(500);
	turn(1, BIN2);
	//usleep(500);
}


static void AStop() {
	turn(0, AIN1);
	//usleep(500);
	turn(0, AIN2);
	//usleep(500);
}
static void BStop() {
	turn(0, BIN1);
	//usleep(500);
	turn(0, BIN2);
	//usleep(500);
}

static void standBy() {
	//printf("standBy\n");
	turn(0, STDBY);
	//usleep(500);
	AStop();
	BStop();
}


static void move() {
	turn(1, STDBY);
	//usleep(500);
}

static void moveForward() {
	AFoward();
	BFoward();
}


static void moveBackward() {
	ABackward();
	BBackward();
}


static void turnLeft() {
	AFoward();
	BBackward();
}

static void turnRight() {
	BFoward();
	ABackward();
}

static void shiftGear(int number) {
	//


	//printf("shift gear: %d\n", number);
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