#ifndef NAVIGATION
#define NAVIGATION
#include "Wiring.h"
#include "Ring.h"
//#define WALL_DISTANCE 	9
#define TURN_DISTANCE 	17
#define TURNING_FACTOR_BACK 	-15
#define TURNING_FACTOR_FRONT  -15
#define STRAIGHT_ANGLE 	1500

#define RINGSIZE 35

#define CARSPEED 11

typedef struct IR{
  int FrontLeft;
  int FrontRight;
  int BackLeft;
  int BackRight;
} IR_Read;

int followRight(float x_front, float x_back);
int followLeft(float x_front, float x_back);
float calculateDistance(int value);
void navigation(void);
int trackStateHandling(int trackstate, int *previous_trackstate, int index,
                       float frontRight, float frontLeft,
                       float backRight, float backLeft);
void convertFullDistance(IR_Read *ir, Data_t * data);
void circleRight(void);
void circleLeft(void);
int trigometricTurnAngle(int forward, int back, int diag);
void displayDistances();
void averageDistances(void);
void convertTrueDistance(IR_Read *ir, Data_t *data);

#endif
