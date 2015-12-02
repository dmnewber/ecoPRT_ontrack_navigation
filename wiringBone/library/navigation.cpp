#include "Wiring.h"

void convertTrueDistance(IR_Read *ir, Data_t *data)
{
  data->backRight = calculateDistance(ir->BackRight);
  data->frontRight = calculateDistance(ir->FrontRight);
  data->backLeft = calculateDistance(ir->BackLeft);
  data->frontLeft = calculateDistance(ir->FrontLeft);
}


static int right(Data_t *current, Data_t *previous)
{
  return STRAIGHT_ANGLE + (TURNING_FACTOR_BACK*(current->backRight-TURN_DISTANCE)
                           + TURNING_FACTOR_FRONT*(current->frontRight-TURN_DISTANCE)
                           + TURNING_FACTOR_FRONT*((current->frontRight-TURN_DISTANCE) - (previous->frontRight-TURN_DISTANCE)));
}

static int left(Data_t *current, Data_t *previous)
{
  return STRAIGHT_ANGLE - (TURNING_FACTOR_BACK*(current->backLeft-TURN_DISTANCE)
                           + TURNING_FACTOR_FRONT*(current->frontLeft-TURN_DISTANCE)
                           + TURNING_FACTOR_FRONT*((current->frontLeft-TURN_DISTANCE) - (previous->frontLeft-TURN_DISTANCE)));
}

static int straight(Data_t *current, Data_t *previous)
{
  return STRAIGHT_ANGLE - (0.4*TURNING_FACTOR_BACK*(current->backLeft-TURN_DISTANCE)
                           + 0.4*TURNING_FACTOR_FRONT*(current->frontLeft-TURN_DISTANCE)
                           + 0.4*TURNING_FACTOR_FRONT*((current->frontLeft-TURN_DISTANCE) - (previous->frontLeft-TURN_DISTANCE)))
                        + (0.4*TURNING_FACTOR_BACK*(current->backRight-TURN_DISTANCE)
                           + 0.4*TURNING_FACTOR_FRONT*(current->frontRight-TURN_DISTANCE)
                           + 0.4*TURNING_FACTOR_FRONT*((current->frontRight-TURN_DISTANCE) - (previous->frontRight-TURN_DISTANCE)));
}

int turnstate;

void navigation(void){
	int i;
  IR_Read ir;
	Data_t data;
  List_t *list=NULL;
  FILE *fp;
  ringInit(&list,RINGSIZE);

  /* Get initial readings */
  for(i=0;i<RINGSIZE;i++)
  {
    /* Read from IRs */
		readIR(&ir);

		/* Convert to distance */
    convertFullDistance(&ir,&data);
    data.trackState = DEFAULT;
    /* Push data onto the ring */
    ringPush(&list,data);

    delay(20);
  }

  //delay(9000);
  setSpeed();
  i=0;
	while(1)
  {
    if(i>200)
    {
      setSpeed();
      i=0;
    }
    data.trackState = DEFAULT;
    if(cooldown(list)) turnstate = STRAIGHT;
		/* Read from IRs */
		readIR(&ir);

		/* Convert to distance */
    convertFullDistance(&ir,&data);


    /* Determine track state */
    trackDetection(list,&data);

    fp = fopen("nav.txt","a");
    if(data.trackState==FORK)
    {
      fprintf(fp,"Fork detected\n");
    }
    else if(data.trackState==MERGELEFT)
    {
      fprintf(fp,"Left merge detected\n");
    }
    else if(data.trackState==MERGERIGHT)
    {
      fprintf(fp,"Right merge detected\n");
    }
    fprintf(fp,"Front left distance: %f, Front right distance: %f\nBack left distance:  %f, Back right distance:  %f\nTurn angle: %d\n\n",data.frontLeft,data.frontRight,data.backLeft,data.backRight,data.turn_angle);
    fclose(fp);

    if(analogRead(AIN4) < 780)
    {
      digitalWrite(P8_7,HIGH);
    }

    ringPush(&list,data);

    if(turnstate==FOLLOWRIGHT)
    {
      data.turn_angle = right(&data,&list->data);
    }
    else if(turnstate==FOLLOWLEFT)
    {
      data.turn_angle = left(&data,&list->data);
    }
    else if(turnstate==STRAIGHT)
    {
      data.turn_angle = straight(&data,&list->data);
    }

		/* Update servo */
	  setSteeringAngle(data.turn_angle);

    // printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    // printf("Front Left Distance: %f, Front Left Sensor: %d\n",data.frontLeft,ir.FrontLeft);
    // printf("Front Right Distance: %f, Front Right Sensor: %d\n",data.frontRight,ir.FrontRight);
    // printf("Back Left Distance: %f, Back Left Sensor: %d\n",data.backLeft,ir.BackLeft);
    // printf("Back Right Distance: %f, Back Right Sensor: %d\n",data.backRight,ir.BackRight);
    // printf("Turn angle calculated: %d\n\n",data.turn_angle);


    delay(50);
    // if(data.trackState==FOLLOWRIGHT) turn_angle=trackStateHandling;
    // else printf("Follow left\n");
    // printf("Cooldown: %d\n",*cooldown);
    i+=1;
	}
}


void circleRight(void)
{
  IR_Read ir;
	Data_t data;
  List_t *list=NULL;
  int i;
  ringInit(&list,RINGSIZE);

  /* Get initial readings */
  for(i=0;i<RINGSIZE;i++)
  {
    /* Read from IRs */
		readIR(&ir);

		/* Convert to distance */
    convertFullDistance(&ir,&data);

    /* Push data onto the ring */
    ringPush(&list,data);

    delay(20);
  }

  delay(15000);
  setCarSpeed(13);
  while(1)
  {
    readIR(&ir);

    convertFullDistance(&ir,&data);

    // data.turn_angle = followRight(data.frontRight,data.backRight);
    data.turn_angle = straight(&data,&list->data);
    setSteeringAngle(data.turn_angle);

    ringPush(&list,data);
  }
}


void circleLeft(void)
{
  IR_Read ir;
	Data_t data;
  List_t *list=NULL;
  int i;
  ringInit(&list,RINGSIZE);

  /* Get initial readings */
  for(i=0;i<RINGSIZE;i++)
  {
    /* Read from IRs */
		readIR(&ir);

		/* Convert to distance */
    convertFullDistance(&ir,&data);

    /* Push data onto the ring */
    ringPush(&list,data);

    delay(20);
  }

  delay(15000);
  setCarSpeed(13);
  while(1)
  {
    readIR(&ir);

    convertFullDistance(&ir,&data);

    data.turn_angle = left(&data,&list->data);
    setSteeringAngle(data.turn_angle);

    ringPush(&list,data);
  }
}

int followRight(float x_front, float x_back)
{
  	int turning_angle;
    if(x_front > x_back)
    {
      turning_angle = STRAIGHT_ANGLE + (0.4*TURNING_FACTOR_FRONT*(x_front-(TURN_DISTANCE))
                                     + 1.3*TURNING_FACTOR_BACK*(x_back-(TURN_DISTANCE)));
    }
    else if(slightlyLargerThan(x_back,x_front))
    {
      turning_angle = STRAIGHT_ANGLE + (1*TURNING_FACTOR_FRONT*(x_front-(TURN_DISTANCE+5))
                                     + 1.3*TURNING_FACTOR_BACK*(x_back-(TURN_DISTANCE)));
    }
    else
    {
      turning_angle = STRAIGHT_ANGLE + (TURNING_FACTOR_FRONT*(x_front-TURN_DISTANCE)
                                     + TURNING_FACTOR_BACK*(x_back-TURN_DISTANCE));
    }

    return turning_angle;
}


int followLeft(float x_front, float x_back)
{
  	int turning_angle;
    if(x_front > x_back)
    {
      turning_angle = STRAIGHT_ANGLE - (0.4*TURNING_FACTOR_FRONT*(x_front-(TURN_DISTANCE))
                                     + 1.3*TURNING_FACTOR_BACK*(x_back-(TURN_DISTANCE)));
    }
    else if(slightlyLargerThan(x_back,x_front))
    {
      turning_angle = STRAIGHT_ANGLE - (1*TURNING_FACTOR_FRONT*(x_front-(TURN_DISTANCE+5))
                                     + 1.3*TURNING_FACTOR_BACK*(x_back-(TURN_DISTANCE)));
    }
    else
    {
      turning_angle = STRAIGHT_ANGLE - (TURNING_FACTOR_FRONT*(x_front-TURN_DISTANCE)
                                     + TURNING_FACTOR_BACK*(x_back-TURN_DISTANCE));
    }

    return turning_angle;
}



float calculateDistance(int value)
{
  float x;
  /* If the value of the ADC is over 185, use regular algorithm */
  if(value>=230)
  {
    x = value*3.6/1024;
    //printf("usual");
    return 2.6867*x*x-15.526*x+25.948;
  }
  /* If the value of the ADC is between 100 and 185, use a shallow
     linear approximation */
  else if(value >= 180 && value < 230)
  {
    //printf("0.1");
    return -0.1*value+38;
  }
  /* If the value of the ADC is below 100, use an expedited linear
     approximation */
  else if(value >= 152 && value < 180)
  {
    //printf("1/7");
    return -0.14285*value + 45.714;
  }
  else if(value >= 125 && value < 152)
  {
    //printf("0.222");
    return -0.22222*value + 57.7775;
  }
  else
  {
    //printf("0.35");
    return -0.35714*value + 74.64258;
  }

}


void convertFullDistance(IR_Read *ir, Data_t * data)
{
  data->backRight = calculateDistance(ir->BackRight);
  data->frontRight = 0.8*calculateDistance(ir->FrontRight);
  data->backLeft = calculateDistance(ir->BackLeft);
  data->frontLeft = 0.8*calculateDistance(ir->FrontLeft);
}



// int trackStateHandling(int trackstate, int *previous_trackstate, int index,
//                        float frontRight, float frontLeft,
//                        float backRight, float backLeft){
//   int turn_angle;
//   int trackStateCheck=0;
//
//   /* Variable to check if the previous state was a fork */
//   trackStateCheck = (*previous_trackstate != FORK
//      && *previous_trackstate != STRAIGHTFORKLEFT
//      && *previous_trackstate != STRAIGHTFORKRIGHT);
//
// 	/* If else construct to handle trackstate */
// 	if(trackstate == DEFAULT){
// 		/* Follow right wall by default */
// 		turn_angle = followRight(frontRight,backRight);
//     /* Set previous track state */
//     *previous_trackstate = DEFAULT;
//     printf("Trackstate DEFAULT\n");
// 	}
//
//   /* Fork handling */
// 	else if(trackstate == FORK){
//     /* Set previous track state */
//     *previous_trackstate = FORK;
// 		/* This is where the index needs to come in */
// 		if(index==0 && trackStateCheck)
//     {
// 			turn_angle = followRight(frontRight,backRight);
// 			index = 1;
// 		}
//     else if(trackStateCheck)
//     {
// 			turn_angle = followLeft(frontLeft,backLeft);
// 			index = 0;
// 		}
//     printf("Trackstate FORK\n");
// 	}
//
//   /* Merge left handling */
// 	else if(trackstate == MERGELEFT){
//     /* Follow right since left is empty space */
// 		turn_angle = followRight(frontRight,backRight);
//
//     /* Set prvious track state */
//     *previous_trackstate = MERGELEFT;
//
//     printf("Trackstate MERGELEFT\n");
// 	}
//
//   /* Merge right handling */
// 	else if(trackstate == MERGERIGHT){
//     /* Follow left since right is empty space */
// 		turn_angle = followLeft(frontLeft,backLeft);
//
//     /* Set previous track state */
//     *previous_trackstate = MERGERIGHT;
//     printf("Trackstate MERGERIGHT\n");
// 	}
//
//   /* Straight left fork handling */
//   else if (trackstate == STRAIGHTFORKLEFT){
//     /* Set previous track state */
//     *previous_trackstate = STRAIGHTFORKLEFT;
//     /* This is where the index needs to come in */
//     if(index==0 && trackStateCheck){
//       turn_angle = followRight(frontRight,backRight);
//       index = 1;
//     } else if(trackStateCheck){
//       turn_angle = followLeft(frontLeft,backLeft);
//       index = 0;
//     }
//     printf("Trackstate STRAIGHTFORKLEFT\n");
//   }
//
//   /* Straight right fork handling */
//   else if(trackstate == STRAIGHTFORKRIGHT){
//     /* Set previous track state */
//     *previous_trackstate = STRAIGHTFORKRIGHT;
//     /* This is where the index needs to come in */
//     if(index==0 && trackStateCheck){
//       turn_angle = followRight(frontRight,backRight);
//       index = 1;
//     } else if(trackStateCheck){
//       turn_angle = followLeft(frontLeft,backLeft);
//       index = 0;
//     }
//     printf("Trackstate STRAIGHTFORKRIGHT\n");
//   }
//
// 	return turn_angle;
// }
