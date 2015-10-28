#include "Wiring.h"



void navigation(void){
	int turn_angle, trackstate = DEFAULT, previous_trackstate, index=0;
  int irFrontLeft, irFrontRight, irBackLeft, irBackRight;
	float backRight, frontRight, backLeft, frontLeft;
  // delay(15000);
	// setCarSpeed(15);
	while(1){
		/* Read from IRs */
		readIR(&irFrontLeft,&irFrontRight,&irBackLeft,&irBackRight);

		/* Convert to distance */
		backRight = calculateDistance(irBackRight);
		frontRight = cos(0.5236)*calculateDistance(irFrontRight);
    backLeft = calculateDistance(irBackLeft);
    frontLeft = cos(0.5236)*calculateDistance(irFrontLeft);
    printf("\n");
    printf("frontLeft: %f, frontRight: %f\n",frontLeft,frontRight);
    printf("backLeft: %f, backRight: %f\n\n",backLeft,backRight);
    /* Determine track state */
    trackstate = trackFeatureDetection(frontRight,frontLeft,backRight,backLeft);

		/* Calculate turn angle */
		turn_angle = trackStateHandling(trackstate,&previous_trackstate,index,frontRight,frontLeft,backRight,backLeft);

		/* Update servo */
		// setSteeringAngle(turn_angle);
    printf("turn angle: %d\n", turn_angle);
    delay(700);
	}
}


void setForkLEDHigh(){
  digitalWrite(P9_42,HIGH);
}

void setForkLEDLow(){
  digitalWrite(P9_42,LOW);
}

void setMergeLEDHigh(){
  digitalWrite(P9_11,HIGH);
}

void setMergeLEDLow(){
  digitalWrite(P9_11,LOW);
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




int followRight(float x_front, float x_back){
  	int turning_angle;
  	turning_angle = STRAIGHT_ANGLE + (TURNING_FACTOR*(x_front-TURN_DISTANCE)
                    + TURNING_FACTOR*(x_back-TURN_DISTANCE));
  	return turning_angle;
}



int followLeft(float x_front, float x_back){
  	int turning_angle;
  	turning_angle = STRAIGHT_ANGLE - (TURNING_FACTOR*(x_front-TURN_DISTANCE)
                    + TURNING_FACTOR*(x_back-TURN_DISTANCE));
  	return turning_angle;
}



float calculateDistance(int value){
  int x;
  /* If the value of the ADC is over 185, use regular algorithm */
  if(value>=185)
  {
    x = value*3.6/1024;
    return 2.6867*x*x-15.526*x+25.948;
  }
  /* If the value of the ADC is between 100 and 185, use a shallow
     linear approximation */
  else if(value >= 100 && value < 185)
  {
    return -0.15294*value+45.29;
  }
  /* If the value of the ADC is below 100, use an expedited linear
     approximation */
  else if(value < 100)
  {
    return -0.4*value + 70;
  }
}





void convertFullDistance(int irFrontLeft, int irFrontRight, int irBackLeft,
                         int irBackRight, float *frontLeft, float * frontRight,
                         float *backLeft, float * backRight){
  *backRight = calculateDistance(irBackRight);
  *frontRight = cos(0.5236)*calculateDistance(irFrontRight);
  *backLeft = calculateDistance(irBackLeft);
  *frontLeft = cos(0.5236)*calculateDistance(irFrontLeft);
}




void circleRight(void){
  int irFrontLeft, irFrontRight, irBackLeft, irBackRight;	// IR reading globals
	float backRight, frontRight, backLeft, frontLeft;
  int turn_angle;
  delay(15000);
  setCarSpeed(20);
  while(1){
    readIR(&irFrontLeft,&irFrontRight,&irBackLeft,&irBackRight);

    convertFullDistance(irFrontLeft,irFrontRight,irBackLeft,irBackRight,
                        &frontLeft,&frontRight,&backLeft,&backRight);

    turn_angle = followRight(frontRight,backRight);

    setSteeringAngle(turn_angle);
  }
}


void circleLeft(void){
  int irFrontLeft, irFrontRight, irBackLeft, irBackRight;	// IR reading globals
	float backRight, frontRight, backLeft, frontLeft;
  int turn_angle;
  delay(15000);
  setCarSpeed(20);
  while(1){
    readIR(&irFrontLeft,&irFrontRight,&irBackLeft,&irBackRight);

    convertFullDistance(irFrontLeft,irFrontRight,irBackLeft,irBackRight,
                        &frontLeft,&frontRight,&backLeft,&backRight);

    turn_angle = followLeft(frontLeft,backLeft);

    setSteeringAngle(turn_angle);
  }
}

#define FACTOR (10/0.5235)
/* A more exact turn calculation based on trigonometry instead of a made up
   algorithm that only sort of works */
int trigometricTurnAngle(int forward, int back, int diag){
  float angle;
  /* Calculate the required turn angle using a derived trigometric function
     and return it as a float */
  angle = atan((forward-13)/(sin(0.5235)*diag));
  return 1500 + angle*FACTOR;
}
