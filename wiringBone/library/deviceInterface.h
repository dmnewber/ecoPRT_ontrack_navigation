#ifndef DEVICE
#define DEVICE

#include "Wiring.h"
#include "navigation.h"

void readIR(IR_Read *ir);
void setSteeringAngle(int pulsewidth);
void setCarSpeed(int speed);
void setSpeed();

#endif
