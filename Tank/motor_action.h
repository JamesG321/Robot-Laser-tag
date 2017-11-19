#ifndef _MOTOR_ACTION_H_
#define _MOTOR_ACTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// PINS
#define AIN1 67
#define AIN2 66
#define BIN1 69
#define BIN2 68
#define STDBY 44
#define FRONTEYE 0
#define LEFTEYE 1
#define RIGHTEYE 4
#define BACKEYE 3

// DUTY for speed
#define ZERO 320000
#define MAX 10000
#define GEAR1 280000
#define GEAR2 220000
#define GEAR3 150000
#define GEAR4 100000
int Gears[6] = {ZERO, GEAR1, GEAR2, GEAR3, GEAR4, MAX};

// number of GPIOs
//#define GPIO 5

// SIGNALS
#define STOP 23
#define MOVE 22
#define LEFT 20
#define RIGHT 21
#define ACTION 24
#define NOACTION 25

// basic action (high level)
static void turn();
static void moveForward();
static void standBy();
static void move();
static void moveBackward();
static void turnLeft();
static void turnRight();
static void shiftGear(int number);

// basic action (low level)
static void AFoward();
static void ABackward();
static void BFoward();
static void BBackward();
static void AStop();
static void BStop();

/*
static void sig_handler(int signo);*/

#endif