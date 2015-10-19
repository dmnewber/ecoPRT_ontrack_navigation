#ifndef DEVICE
#define DEVICE

void readIR(int *irForwardLeft, int *irForwardRight,
            int *irBackLeft, int *irBackRight);
void setSteeringAngle(int pulsewidth);
void setCarSpeed(int speed);

#endif
