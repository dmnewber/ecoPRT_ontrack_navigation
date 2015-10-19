#include "Wiring.h"


void navigation(void){
	int turn_angle, trackstate;
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
		turn_angle = trackStateHandling(trackstate,frontRight,frontLeft,backRight,backLeft);

		/* Update servo */
		setSteeringAngle(turn_angle);
	}
}

int trackStateHandling(int trackstate, float frontRight, float frontLeft,
                       float backRight, float backLeft){
  int turn_angle, alternate=0;
	/* If else construct to handle trackstate */
	if(trackstate == DEFAULT){
		/* Follow right wall by default */
		turn_angle = followRight(frontRight,backRight);
	}
	else if(trackstate == FORK){
		/* Indicate with LED */
		digitalWrite(P9_42, HIGH);
    delay(300);
    digitalWrite(P9_42, LOW);

		/* This is where the index needs to come in */
		if(alternate==0){
			turn_angle = followRight(frontRight,backRight);
			alternate = 1;
		} else{
			turn_angle = followLeft(frontLeft,backLeft);
			alternate = 0;
		}
	}
	else if(trackstate == MERGELEFT){
		turn_angle = followRight(frontRight,backRight);
	}
	else if(trackstate == MERGERIGHT){
		turn_angle = followLeft(frontLeft,backLeft);
	}
  else if (trackstate == STRAIGHTFORKLEFT){
    digitalWrite(P9_42, HIGH);
    delay(300);
    digitalWrite(P9_42, LOW);
  }
  else if(trackstate == STRAIGHTFORKRIGHT){
    digitalWrite(P9_42, HIGH);
    delay(300);
    digitalWrite(P9_42, LOW);
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
