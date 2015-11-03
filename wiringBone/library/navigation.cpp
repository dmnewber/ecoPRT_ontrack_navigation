#include "Wiring.h"



static void convertTrueDistance(IR_Read *ir, Data_t *data){
  data->backRight = calculateDistance(ir->BackRight);
  data->frontRight = calculateDistance(ir->FrontRight);
  data->backLeft = calculateDistance(ir->BackLeft);
  data->frontLeft = calculateDistance(ir->FrontLeft);
}

void navigation(void){
	int index=0;
  IR_Read ir;
	Data_t data;
  List_t list;


  // delay(15000);
	// setCarSpeed(15);
	while(1)
  {
		/* Read from IRs */
		readIR(&ir);

		/* Convert to distance */
    convertFullDistance(&ir,&data);

    /* Determine track state */
    trackstate = trackFeatureDetection(frontRight,frontLeft,backRight,backLeft);

		/* Calculate turn angle */
		turn_angle = trackStateHandling(trackstate,&previous_trackstate,index,frontRight,frontLeft,backRight,backLeft);

		/* Update servo */
	  setSteeringAngle(turn_angle);
	}
}



void circleRight(void)
{
  IR_Read ir;
	Data_t data;

  delay(15000);
  setCarSpeed(20);
  while(1)
  {
    readIR(&ir);

    convertFullDistance(&ir,&data);

    data.turn_angle = followRight(data.frontRight,data.backRight);

    setSteeringAngle(data.turn_angle);
  }
}


void circleLeft(void)
{
  IR_Read ir;
	Data_t data;
  delay(15000);
  setCarSpeed(20);
  while(1)
  {
    readIR(&ir);

    convertFullDistance(&ir,&data);

    turn_angle = followLeft(data.frontLeft,data.backLeft);

    setSteeringAngle(data.turn_angle);
  }
}

int followRight(float x_front, float x_back)
{
  	int turning_angle;
    if(x_front > x_back)
    {
      turning_angle = STRAIGHT_ANGLE + (TURNING_FACTOR_FRONT*(x_front-(TURN_DISTANCE-2))
                      + TURNING_FACTOR_BACK*(x_back-(TURN_DISTANCE+2)));
    }
    else
    {
      turning_angle = STRAIGHT_ANGLE + (TURNING_FACTOR_FRONT*(x_front-TURN_DISTANCE)
                      + TURNING_FACTOR_BACK*(x_back-TURN_DISTANCE));
    }

  	return turning_angle;
}



int followLeft(float x_front, float x_back)
{
  	int turning_angle;
    if(x_front > x_back)
    {
      turning_angle = STRAIGHT_ANGLE - (TURNING_FACTOR_FRONT*(x_front-(TURN_DISTANCE-2))
                      + TURNING_FACTOR_BACK*(x_back-(TURN_DISTANCE+2)));
    }
    else
    {
      turning_angle = STRAIGHT_ANGLE - (TURNING_FACTOR_FRONT*(x_front-TURN_DISTANCE)
                      + TURNING_FACTOR_BACK*(x_back-TURN_DISTANCE));
    }

  	return turning_angle;
}



float calculateDistance(int value)
{
  float x;
  /* If the value of the ADC is over 185, use regular algorithm */
  if(value>=230)
  {
    x = value*3.6/1024;
    //printf("usual");
    return 2.6867*x*x-15.526*x+25.948;
  }
  /* If the value of the ADC is between 100 and 185, use a shallow
     linear approximation */
  else if(value >= 180 && value < 230)
  {
    //printf("0.1");
    return -0.1*value+38;
  }
  /* If the value of the ADC is below 100, use an expedited linear
     approximation */
  else if(value >= 152 && value < 180)
  {
    //printf("1/7");
    return -0.14285*value + 45.714;
  }
  else if(value >= 125 && value < 152)
  {
    //printf("0.222");
    return -0.22222*value + 57.7775;
  }
  else
  {
    //printf("0.35");
    return -0.35714*value + 74.64258;
  }

}


void convertFullDistance(IR_Read *ir, Data_t * data)
{
  data->backRight = calculateDistance(ir->BackRight);
  data->frontRight = cos(0.5236)*calculateDistance(ir->FrontRight);
  data->backLeft = calculateDistance(ir->BackLeft);
  data->frontLeft = cos(0.5236)*calculateDistance(ir->FrontLeft);
}



int trackStateHandling(int trackstate, int *previous_trackstate, int index,
                       float frontRight, float frontLeft,
                       float backRight, float backLeft){
  int turn_angle;
  int trackStateCheck=0;

  /* Variable to check if the previous state was a fork */
  trackStateCheck = (*previous_trackstate != FORK
     && *previous_trackstate != STRAIGHTFORKLEFT
     && *previous_trackstate != STRAIGHTFORKRIGHT);

	/* If else construct to handle trackstate */
	if(trackstate == DEFAULT){
		/* Follow right wall by default */
		turn_angle = followRight(frontRight,backRight);
    setMergeLEDLow();
    /* Set previous track state */
    *previous_trackstate = DEFAULT;
    printf("Trackstate DEFAULT\n");
	}

  /* Fork handling */
	else if(trackstate == FORK){
    /* Set previous track state */
    *previous_trackstate = FORK;
    setMergeLEDLow();
		/* This is where the index needs to come in */
		if(index==0 && trackStateCheck)
    {
			turn_angle = followRight(frontRight,backRight);
			index = 1;
      setForkLEDHigh();
		}
    else if(trackStateCheck)
    {
			turn_angle = followLeft(frontLeft,backLeft);
			index = 0;
      setForkLEDLow();
		}
    printf("Trackstate FORK\n");
	}

  /* Merge left handling */
	else if(trackstate == MERGELEFT){
    /* Follow right since left is empty space */
		turn_angle = followRight(frontRight,backRight);

    /* Set prvious track state */
    *previous_trackstate = MERGELEFT;

    setMergeLEDHigh();
    printf("Trackstate MERGELEFT\n");
	}

  /* Merge right handling */
	else if(trackstate == MERGERIGHT){
    /* Follow left since right is empty space */
		turn_angle = followLeft(frontLeft,backLeft);

    /* Set previous track state */
    *previous_trackstate = MERGERIGHT;
    setMergeLEDHigh();
    printf("Trackstate MERGERIGHT\n");
	}

  /* Straight left fork handling */
  else if (trackstate == STRAIGHTFORKLEFT){
    /* Set previous track state */
    *previous_trackstate = STRAIGHTFORKLEFT;
    setMergeLEDLow();
    /* This is where the index needs to come in */
    if(index==0 && trackStateCheck){
      turn_angle = followRight(frontRight,backRight);
      index = 1;
      setForkLEDHigh();
    } else if(trackStateCheck){
      turn_angle = followLeft(frontLeft,backLeft);
      index = 0;
      setForkLEDLow();
    }
    printf("Trackstate STRAIGHTFORKLEFT\n");
  }

  /* Straight right fork handling */
  else if(trackstate == STRAIGHTFORKRIGHT){
    /* Set previous track state */
    *previous_trackstate = STRAIGHTFORKRIGHT;
    setMergeLEDLow();
    /* This is where the index needs to come in */
    if(index==0 && trackStateCheck){
      turn_angle = followRight(frontRight,backRight);
      index = 1;
      setForkLEDHigh();
    } else if(trackStateCheck){
      turn_angle = followLeft(frontLeft,backLeft);
      index = 0;
      setForkLEDLow();
    }
    printf("Trackstate STRAIGHTFORKRIGHT\n");
  }

	return turn_angle;
}
