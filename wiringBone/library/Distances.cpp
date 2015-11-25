
#include "Wiring.h"


void displayDistances(void)
{
  IR_Read ir;
  Data_t data;
  int turn_angle;
  while(1){
    readIR(&ir);

     convertFullDistance(&ir,&data);

    turn_angle = followLeft(data.frontLeft,data.backLeft);
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("Front Left Distance: %f, Front Left Sensor: %d\n",data.frontLeft,ir.FrontLeft);
    printf("Front Right Distance: %f, Front Right Sensor: %d\n",data.frontRight,ir.FrontRight);
    printf("Back Left Distance: %f, Back Left Sensor: %d\n",data.backLeft,ir.BackLeft);
    printf("Back Right Distance: %f, Back Right Sensor: %d\n",data.backRight,ir.BackRight);
    printf("Turn angle calculated: %d\n\n",turn_angle);

    delay(500);
  }

}

void averageDistances(void)
{
  IR_Read ir;
  int i;
  float frAverage,flAverage,brAverage,blAverage;
  int frMax=0, frMin=5000, flMax=0, flMin=50000, brMax=0, brMin=50000, blMax=0, blMin=50000;
  int fr[500], fl[500], br[500],bl[500];
  for(i=0;i<500;i++)
  {
    readIR(&ir);
    fr[i] = ir.FrontRight;
    fl[i] = ir.FrontLeft;
    br[i] = ir.BackRight;
    bl[i] = ir.BackLeft;
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
