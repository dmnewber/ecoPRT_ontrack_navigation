#include "deviceInterface.h"

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


void readIR(IR_Read *ir){
	/* Read forward left IR */
	ir->FrontLeft = analogRead(AIN0);

	/* Read forward right IR */
	ir->FrontRight = analogRead(AIN1);

	/* Read back left IR */
	ir->BackLeft = analogRead(AIN2);

	/* Read back right IR */
	ir->BackRight = analogRead(AIN3);
}

void setSpeed()
{
  setCarSpeed(1.2/(analogRead(AIN4)*0.0115)*100);
}
