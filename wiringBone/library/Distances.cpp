
#include "wiring.h"


void displayDistances(void)
{
  int irFrontLeft, irFrontRight, irBackLeft, irBackRight;	// IR reading globals
	float backRight, frontRight, backLeft, frontLeft;
  int turn_angle;
  while(1){
    readIR(&irFrontLeft,&irFrontRight,&irBackLeft,&irBackRight);

    convertTrueDistance(irFrontLeft,irFrontRight,irBackLeft,irBackRight,
                        &frontLeft,&frontRight,&backLeft,&backRight);

    turn_angle = followLeft(frontLeft,backLeft);
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("Front Left Distance: %f, Front Left Sensor: %d\n",frontLeft,irFrontLeft);
    printf("Front Right Distance: %f, Front Right Sensor: %d\n",frontRight,irFrontRight);
    printf("Back Left Distance: %f, Back Left Sensor: %d\n",backLeft,irBackLeft);
    printf("Back Right Distance: %f, Back Right Sensor: %d\n",backRight,irBackRight);
    printf("Turn angle calculated: %d\n\n",turn_angle);

    delay(500);
  }

}

void averageDistances(void)
{
  int i,irFrontLeft, irFrontRight, irBackLeft, irBackRight;	// IR reading globals
  float backRight, frontRight, backLeft, frontLeft;
  int turn_angle;
  float frAverage,flAverage,brAverage,blAverage;
  int frMax=0, frMin=5000, flMax=0, flMin=50000, brMax=0, brMin=50000, blMax=0, blMin=50000;
  int fr[500], fl[500], br[500],bl[500];
  for(i=0;i<500;i++)
  {
    readIR(&irFrontLeft,&irFrontRight,&irBackLeft,&irBackRight);
    fr[i] = irFrontRight;
    fl[i] = irFrontLeft;
    br[i] = irBackRight;
    bl[i] = irBackLeft;
    delay(20);
  }
  for(i=0;i<500;i++)
  {
    frAverage += fr[i];
    flAverage += fl[i];
    brAverage += br[i];
    blAverage += bl[i];
    frMax = max(frMax,fr[i]);
    frMin = min(frMin,fr[i]);
    flMax = max(flMax,fl[i]);
    flMin = min(flMin,fl[i]);
    brMax = max(brMax,br[i]);
    brMin = min(brMin,br[i]);
    blMax = max(blMax,bl[i]);
    blMin = min(blMin,bl[i]);
  }
  frAverage = frAverage/500;
  flAverage = flAverage/500;
  brAverage = brAverage/500;
  blAverage = blAverage/500;

  printf("Front Right Average: %f, Front Right Max: %d, Front Right Min: %d\n",frAverage,frMax,frMin);
  printf("Front Left Average: %f, Front Left Max: %d, Front Left Min: %d\n",flAverage,flMax,flMin);
  printf("Back Right Average: %f, Back Right Max: %d, Back Right Min: %d\n",brAverage,brMax,brMin);
  printf("Back Left Average: %f, Back Left Max: %d, Back Left Min: %d\n\n",blAverage,blMax,blMin);
}
