int x;

void setSteeringAngle(int pulsewidth);
void setCarSpeed(int speed);

#define MOTOR_FREQUENCY	10
#define	SERVO_PERIOD	20000


void setup(){
	setTimePeriod(P9_27,SERVO_PERIOD);
	setPulseWidth(P9_27,0);
	setTimePeriod(P9_41,MOTOR_FREQUENCY);
	setDutyPercentage(P9_41,0);
}
int lidar;
void loop(){
/*
	printf("Enter car speed percentage: ");
	scanf("%d",&x);
	setCarSpeed(x);
	delay(500);
*/

	/* Lidar reading test */
	lidar = getPulseWidth(P9_42);
	printf("%d\n",lidar);
	delay(10);
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
	setDutyPercentage(P9_41,speed);
}
