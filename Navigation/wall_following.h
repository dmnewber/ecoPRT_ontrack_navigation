/*
* Defines
*   WALL_DISTANCE is the WALL_DISTANCE is the desired distance from the walls in cm.
*   TURNING_FACTOR is the multiplier given to the PI algorithm.
*   STRAIGHT_ANGLE is the number given to the motor control board
*   to keep the wheels straight.
*/


#define WALL_DISTANCE 15
#define TURN_DISTANCE 10
#define TURNING_FACTOR 3
#define STRAIGHT_ANGLE 127

int straight(int x_front, int x_back);
int turn(int x_front, int x_back);
