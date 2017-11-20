//Robot laser tag game
//James Guo
//Autmn 2016
//Signals.h

//defines ascii sequence for different commands between the controller and the robot.
//e.g if the controller says move forward, it sends string "11" to the robot via bluetooth communication protocol.

#define FORWARD1 11
#define FORWARD3 12

#define BACKWARD 22
#define STANDBY 99
#define LEFT 20
#define RIGHT 21

#define HIT 8
#define EYE1 5
#define EYE2 6