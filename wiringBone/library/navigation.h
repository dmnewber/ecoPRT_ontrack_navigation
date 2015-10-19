#ifndef NAVIGATION
#define NAVIGATION

#define WALL_DISTANCE 	15
#define TURN_DISTANCE 	13
#define TURNING_FACTOR 	-18
#define STRAIGHT_ANGLE 	1500


int followRight(float x_front, float x_back);
int followLeft(float x_front, float x_back);
float calculateDistance(float value);
void navigation(void);
int trackStateHandling(int trackstate, float frontRight, float frontLeft,
                       float backRight, float backLeft);

#endif
