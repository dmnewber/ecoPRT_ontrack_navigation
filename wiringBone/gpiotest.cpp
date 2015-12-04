
/* defines */
#define MOTOR_FREQUENCY	20000
#define	SERVO_PERIOD	20000

/* Running code */

void setup(){
	setTimePeriod(P9_27,SERVO_PERIOD);
	setPulseWidth(P9_27,1500);
	setTimePeriodns(P9_21,50000);
	analogReadResolution(10);
	/* Output pin for fork detection LED */
	pinMode(P8_7,OUTPUT);
  pinMode(P8_9,OUTPUT);
  pinMode(P8_11,OUTPUT);
}


void loop(){
  //circleRight();
  //circleLeft();
  //displayDistances();
  navigation();
  //averageDistances();
  //printf("read: %d\n",analogRead(AIN4));
  //delay(300);
}
