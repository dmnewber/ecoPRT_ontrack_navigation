/* Integer declarations */
int irForwardLeft, irForwardRight, irBackLeft, irBackRight;	// IR reading globals

/* Function declarations */
void readIR(void);
void setSteeringAngle(int pulsewidth);
void setCarSpeed(int speed);
int turn(float x_front, float x_back);
float calculateDistance(float value);
void navigation(void);
void testingInterface(void);

/* defines */
#define MOTOR_FREQUENCY	20000
#define	SERVO_PERIOD	20000
#define WALL_DISTANCE 	15
#define TURN_DISTANCE 	13
#define TURNING_FACTOR 	-18
#define STRAIGHT_ANGLE 	1500


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

	/* Read back right IR */
	irBackRight = analogRead(AIN5);
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
	setCarSpeed(25);
	while(1){
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
}

void testingInterface(void){
	int user, x, check=0; 
	float front,back;
	printf("HELLLLOOOOOO Alpha Demo!\n");
	delay(1500);
	printf("This is the ecoPRT Ontrack Navigation Alpha Demo Interface!\n");
	delay(1500);
	while(1){
		printf("What would you like to do?\n");
		printf("1 - Demonstrate the servo.\n");
		printf("2 - Demonstrate the motor control.\n");
		printf("3 - Demonstrate the IR rangefinder readings for 10 seconds.\n");
		printf("4 - Run the navigation algorithm (do this last)\n");
		printf("Enter the number of your desired option: ");
		scanf("%d",&user);
		if(user == 1){
			printf("This is to demonstrate servo/steering functionality.\n");
			printf("The servo takes a pulse from the PRU that ranges from 1ms to 2ms.\n");
			printf("However, due to the phyiscal limitations on steering, only a certain\n");
			printf("range is applicable. This has been handled in code and no value out of\n");
			printf("range will force the servo to go past the car's physical limit.\n");
			printf("Enter in your pulse widths in micro seconds.\n");
			printf("This section will allow 5 entries per selection\n");
			for(int i=0;i<5;i++){
				printf("Enter servo pulse width: ");
				scanf("%d",&x);
				setSteeringAngle(x);
				delay(500);
			}
		} else if(user == 2){
			printf("This is to demonstrate motor control.\n");
			printf("The motor control is handled by a PWM coming from the PRU.\n");
			printf("The function that sets the motor speed does so with a\n");
			printf("duty cycle that is set as the PWM duty cycle going to the\n");
			printf("motor control board. Accepted values are 0-100.\n");
			printf("This section will allow 5 entries per selection\n");
			for(int i=0;i<5;i++){
				while(!check){
					printf("Enter car speed percentage: ");
					scanf("%d",&x);
					if(x<=100 && x>=0) check=1;
					else printf("Please enter a value between 0 and 100\n");
				}
				check=0;
				setCarSpeed(x);
				delay(500);	
			}
			setCarSpeed(0);
		} else if(user == 3){
			printf("This is to demonstrate the IR reading. IR reading is handled\n");
			printf("through the ADC reading the analog voltage outputted by the\n");
			printf("IR sensors. In a moment, the values being read from the sensors\n");
			printf("will be printed here in cm for ten seconds. Feel free to play with\n");
			printf("the sensors to make sure they are actually outputting reasonable data!\n\n");
			delay(5000);
			for(int i=0;i<100;i++){
				readIR();
	 	                back = calculateDistance(irBackRight*2*1.8/1024);
                		front = cos(0.5236)*calculateDistance(irForwardRight*2*1.8/1024);
				printf("\nForward right sensor: %f\nBack right sensor: %f\n",front, back);
				delay(100);
			}
		} else if(user == 4){
			navigation();
		}
	}
}
