/* Integer declarations */

/* Function declarations */
void readIR(void);
void setSteeringAngle(int pulsewidth);
void setCarSpeed(int speed);
int followRight(float x_front, float x_back);
int followLeft(float x_front, float x_back);
float calculateDistance(float value);
void navigation(void);

/* defines */
#define MOTOR_FREQUENCY	20000
#define	SERVO_PERIOD	20000
#define WALL_DISTANCE 	15
#define TURN_DISTANCE 	13
#define TURNING_FACTOR 	-18
#define STRAIGHT_ANGLE 	1500

/* Running code */

void setup(){
	setTimePeriod(P9_27,SERVO_PERIOD);
	setPulseWidth(P9_27,1500);
	setFrequency(P9_41,MOTOR_FREQUENCY);
	setDutyPercentage(P9_41,0);
	analogReadResolution(10);
}

void loop(){
	testingInterface();
}

/* Function definitions */


void navigation(void){
	int turn_angle, trackstate, alternate=0 /* temporary variable to test fork handling */;
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

    /* If else construct to handle trackstate */
    if(trackstate == DEFAULT){
      /* Follow right wall by default */
      turn_angle = followRight(frontRight,backRight);
    }
    else if(trackstate == FORK){
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

		/* Update servo */
		setSteeringAngle(turn_angle);
	}
}




void setSteeringAngle(int pulsewidth){
	/* Ensure the pulse width isn't too low */
	pulsewidth = max(1200,pulsewidth);

	/* Ensure the pulse width isn't too high */
	pulsewidth = min(1750,pulsewidth);

	/* Set steering pulsewidth */
	setPulseWidth(P9_27,pulsewidth);
}





void setCarSpeed(int speed){
	/* Set car speed */
	int i = 0;
	for(i=0;i<=speed;i++){
		setDutyPercentage(P9_41,i);
		delay(3);
	}
}





void readIR(int *irForwardLeft, int *irForwardRight,
            int *irBackLeft, int *irBackRight){
	/* Read forward left IR */
	*irForwardLeft = analogRead(AIN0);

	/* Read forward right IR */
	*irForwardRight = analogRead(AIN1);

	/* Read back left IR */
	*irBackLeft = analogRead(AIN2);

	/* Read back right IR */
	*irBackRight = analogRead(AIN5);
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
