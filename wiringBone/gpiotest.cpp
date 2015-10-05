/* Integer declarations */
int x, lidar;
int irForwardLeft, irForwardRight, irBackLeft, irBackRight;

/* Function declarations */
void readIR(void);
void setSteeringAngle(int pulsewidth);
void setCarSpeed(int speed);
int turn(int x_front, int x_back);
int calculateDistance(int value);

/* defines */
#define MOTOR_PERIOD	10
#define	SERVO_PERIOD	20000
#define WALL_DISTANCE 	15
#define TURN_DISTANCE 	13
#define TURNING_FACTOR 	-18
#define STRAIGHT_ANGLE 	1500


void setup(){
	setTimePeriod(P9_27,SERVO_PERIOD);
	setPulseWidth(P9_27,1500);
	setTimePeriod(P9_41,MOTOR_PERIOD);
	setDutyPercentage(P9_41,0);
	delay(5000);
//	setCarSpeed(30);
}

void loop(){
/*
	printf("Enter car speed percentage: ");
	scanf("%d",&x);
	setCarSpeed(x);
	delay(500);
*/
/*
	printf("Enter servo pulse width: ");
	scanf("%d",&x);
	setSteeringAngle(x);
	delay(500);
*/

	/* Lidar reading test */
/*	lidar = getPulseWidth(P9_42);
	printf("%d\n",lidar);
	delay(10);
*/

	/* Read all four IRs */
	readIR();
//	x = analogRead(AIN0);

	/* Print IR values */
	printf("Front left: %d, Front right: %d, Back left: %d, Back right: %d\n", irForwardLeft, irForwardRight, irBackLeft, irBackRight);
//	printf("Analog read: %d\n", x);

	/* Delay 200ms */
	delay(200);
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
	for(i=0; i<=speed;i++){
		setDutyPercentage(P9_41,i);
		delay(40);
	}
}

void readIR(void){
	/* Read forward left IR */
	irForwardLeft = analogRead(AIN0);

	/* Read forward right IR */
	irForwardRight = analogRead(AIN1);

	/* Read back left IR */
	irBackLeft = analogRead(AIN2);

	/* Read back left IR */
	irBackRight = analogRead(AIN3);
}


int turn(int x_front, int x_back){
  int turning_angle;
  turning_angle = STRAIGHT_ANGLE + (TURNING_FACTOR*(x_front-TURN_DISTANCE) + TURNING_FACTOR*(x_back-TURN_DISTANCE));
  return turning_angle;
}

int calculateDistance(int value){
  return 2.6867*value*value-15.526*value+25.948;
}

