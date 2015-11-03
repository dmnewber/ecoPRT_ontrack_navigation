#ifndef NAVIGATION
#define NAVIGATION

#define WALL_DISTANCE 	13
#define TURN_DISTANCE 	13
#define TURNING_FACTOR_BACK 	-15
#define TURNING_FACTOR_FRONT  -15
#define STRAIGHT_ANGLE 	1500

typedef struct{
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
void setForkLEDHigh();
void setForkLEDLow();
void setMergeLEDHigh();
void setMergeLEDLow();
void displayDistances();
void averageDistances(void);

#endif
