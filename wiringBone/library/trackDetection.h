/*
Header file for trackDetection.cpp
*/

#ifndef TRACKDETECTION
#define TRACKDETECTION

#include "Ring.h"

#define DEFAULT            1
#define FORK               2
#define MERGELEFT          3
#define MERGERIGHT         4
#define STRAIGHTFORKLEFT   5
#define STRAIGHTFORKRIGHT  6

#define FOLLOWRIGHT 1
#define FOLLOWLEFT  2

#define COOLDOWN 1

int trackFeatureDetection(Data_t * data);
void trackDetection(List_t *list, Data_t * data, int *cooldown);
#endif
