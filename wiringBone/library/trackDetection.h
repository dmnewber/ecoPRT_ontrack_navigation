/*
Header file for trackDetection.cpp
*/

#ifndef TRACKDETECTION
#define TRACKDETECTION


#include "Ring.h"
#include <stdio.h>

#define DEFAULT            1
#define FORK               2
#define MERGELEFT          3
#define MERGERIGHT         4
#define STRAIGHTFORKLEFT   5
#define STRAIGHTFORKRIGHT  6

#define FOLLOWRIGHT 1
#define FOLLOWLEFT  2

#define COOLDOWN 50
#define THRESHOLD 0.55

int trackFeatureDetection(Data_t * data);
void trackDetection(List_t *list, Data_t * data, int *cooldown);
float maximum(float right, float left);
void setForkLEDHigh();
void setForkLEDLow();
void setMergeLEDHigh();
void setMergeLEDLow();
#endif
