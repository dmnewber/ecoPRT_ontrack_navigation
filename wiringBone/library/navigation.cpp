#include "Wiring.h"


void navigation(void){
	int turn_angle, trackstate, previous_trackstate, index=0;
  int irForwardLeft, irForwardRight, irBackLeft, irBackRight;	// IR reading globals
	float backRight, frontRight, backLeft, frontLeft;
	setCarSpeed(25);
	while(1){
		/* Read from IRs */
		readIR(&irForwardLeft,&irForwardRight,&irBackLeft,&irBackRight);

		/* Convert to distance */
		backRight = calculateDistance(irBackRight*2*1.8/1024);
		frontRight = cos(0.5236)*calculateDistance(irForwardRight*2*1.8/1024);
    backLeft = calculateDistance(irBackLeft*2*1.8/1024);
    frontLeft = cos(0.5236)*calculateDistance(irForwardLeft*2*1.8/1024);

    /* Determine track state */
    trackstate = trackFeatureDetection(frontRight,frontLeft,backRight,backLeft);

		/* Calculate turn angle */
		turn_angle = trackStateHandling(trackstate,&previous_trackstate,index,frontRight,frontLeft,backRight,backLeft);

		/* Update servo */
		setSteeringAngle(turn_angle);
	}
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

    /* Set previous track state */
    *previous_trackstate = DEFAULT;
	}

  /* Fork handling */
	else if(trackstate == FORK){
    /* Set previous track state */
    *previous_trackstate = FORK;
		/* This is where the index needs to come in */
		if(index==0 && trackStateCheck)
    {
			turn_angle = followRight(frontRight,backRight);
			index = 1;
		}
    else if(trackStateCheck)
    {
			turn_angle = followLeft(frontLeft,backLeft);
			index = 0;
		}
	}

  /* Merge left handling */
	else if(trackstate == MERGELEFT){
    /* Follow right since left is empty space */
		turn_angle = followRight(frontRight,backRight);

    /* Set prvious track state */
    *previous_trackstate = MERGELEFT;
	}

  /* Merge right handling */
	else if(trackstate == MERGERIGHT){
    /* Follow left since right is empty space */
		turn_angle = followLeft(frontLeft,backLeft);

    /* Set previous track state */
    *previous_trackstate = MERGERIGHT;
	}

  /* Straight left fork handling */
  else if (trackstate == STRAIGHTFORKLEFT){
    /* Set previous track state */
    *previous_trackstate = STRAIGHTFORKLEFT;
    /* This is where the index needs to come in */
    if(index==0 && trackStateCheck){
      turn_angle = followRight(frontRight,backRight);
      index = 1;
    } else if(trackStateCheck){
      turn_angle = followLeft(frontLeft,backLeft);
      index = 0;
    }
  }

  /* Straight right fork handling */
  else if(trackstate == STRAIGHTFORKRIGHT){
    /* Set previous track state */
    *previous_trackstate = STRAIGHTFORKRIGHT;
    /* This is where the index needs to come in */
    if(index==0 && trackStateCheck){
      turn_angle = followRight(frontRight,backRight);
      index = 1;
    } else if(trackStateCheck){
      turn_angle = followLeft(frontLeft,backLeft);
      index = 0;
    }
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
                    - TURNING_FACTOR*(x_back-TURN_DISTANCE));
  	return turning_angle;
}




float calculateDistance(float value){
  	return 2.6867*value*value-15.526*value+25.948;
}
