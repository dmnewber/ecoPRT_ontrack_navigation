#ifndef NAVIGATION
#define NAVIGATION

#define WALL_DISTANCE 	13
#define TURN_DISTANCE 	13
#define TURNING_FACTOR 	-22
#define STRAIGHT_ANGLE 	1500


int followRight(float x_front, float x_back);
int followLeft(float x_front, float x_back);
float calculateDistance(int value);
void navigation(void);
int trackStateHandling(int trackstate, int *previous_trackstate, int index,
                       float frontRight, float frontLeft,
                       float backRight, float backLeft);
void convertFullDistance(int irForwardLeft, int irForwardRight, int irBackLeft,
                         int irBackRight, float *frontLeft, float * frontRight,
                         float *backLeft, float * backRight);
void circleRight(void);
void circleLeft(void);
int trigometricTurnAngle(int forward, int back, int diag);
void setForkLEDHigh();
void setForkLEDLow();
void setMergeLEDHigh();
void setMergeLEDLow();
#endif
