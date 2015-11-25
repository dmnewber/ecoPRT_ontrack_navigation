
/* defines */
#define MOTOR_FREQUENCY	20000
#define	SERVO_PERIOD	20000

/* Running code */

void setup(){
	setTimePeriod(P9_27,SERVO_PERIOD);
	setPulseWidth(P9_27,1500);
	setFrequency(P9_41,MOTOR_FREQUENCY);
	setDutyPercentage(P9_41,0);
	analogReadResolution(10);
	/* Output pin for fork detection LED */
	pinMode(P9_42,OUTPUT);
  pinMode(P9_11,OUTPUT);
//  setMergeLEDHigh();
//  setForkLEDHigh();
}

void loop(){
  //circleRight();
  //circleLeft();
  //displayDistances();
  navigation();
  //averageDistances();

}
