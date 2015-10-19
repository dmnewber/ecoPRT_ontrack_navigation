#include "Wiring.h"


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
	*irBackRight = analogRead(AIN3);
}
