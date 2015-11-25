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
#define LEFTFORK           5
#define RIGHTFORK          6

#define FOLLOWRIGHT 1
#define FOLLOWLEFT  2
#define STRAIGHT 3

#define COOLDOWN 100
#define THRESHOLD 0.5
#define GRADIENT 1

int trackFeatureDetection(Data_t * data);
void trackDetection(List_t *list, Data_t * data);
float maximum(float right, float left);
int slightlyLargerThan(float one, float two);
void setForkLEDHigh();
void setForkLEDLow();
void setMergeLEDHigh();
void setMergeLEDLow();
int cooldown(List_t *list);
#endif
