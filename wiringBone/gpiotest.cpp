/* Integer declarations */
int x, lidar;
int irForwardLeft, irForwardRight, irBackLeft, irBackRight;

/* Function declarations */
void readIR(void);
void setSteeringAngle(int pulsewidth);
void setCarSpeed(int speed);
int turn(float x_front, float x_back);
float calculateDistance(float value);
void navigation(void);

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
	analogReadResolution(10);
	delay(15000);
	setCarSpeed(20);
}

void loop(){
	navigation();
	/* Delay 200ms */
//	delay(200);
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

int turn(float x_front, float x_back){
  	int turning_angle;
  	turning_angle = STRAIGHT_ANGLE + (TURNING_FACTOR*(x_front-TURN_DISTANCE) + TURNING_FACTOR*(x_back-TURN_DISTANCE));
  	return turning_angle;
}

float calculateDistance(float value){
  	return 2.6867*value*value-15.526*value+25.948;
}

void navigation(void){
	int turn_angle;
	float back, front;

	/* Read from IRs */
	readIR();

	/* Convert to distance */
	back = calculateDistance(irBackRight*2*1.8/1024);
	front = cos(0.5236)*calculateDistance(irForwardRight*2*1.8/1024);

	/* Calculate turn angle */
	turn_angle = turn(front,back);

	/* Update servo */
	setSteeringAngle(turn_angle);
}

void testingInterface(void){
        printf("Enter car speed percentage: ");
        scanf("%d",&x);
        setCarSpeed(x);
        delay(500);
        printf("Enter servo pulse width: ");
        scanf("%d",&x);
        setSteeringAngle(x);
        delay(500);
        readIR();
        x = analogRead(AIN0);

        /* Print IR values */
      	printf("Analog read: %d\n", x);

}
