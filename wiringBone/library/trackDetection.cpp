/* track feature detection
Takes all four range finder readings and returns an int representing
the track feature present.

1 - no feature
2 - fork
3 - mergeLeft
4 - mergeRight

1 - default return.
2 - when the two forward distances are larger than the two current
    distances.
3 - when the forwardLeft distance is much larger than the other three.
4 - when the forwardRight distance is much larger than the other three.

Inputs:
  int forwardRight - distance on the front right side.
  int forwardLeft - distance on the front left side.
  int backRight - distance on the back right side.
  int backLeft - distance on the back left side.

Using these inputs, the function determines what track feature the
vehicle is at.
*/
#include "Wiring.h"
#include "trackDetection.h"
#include <math.h>



void setMergeLEDHigh()
{
  digitalWrite(P8_11, HIGH);
}

void setForkLEDHigh()
{
  digitalWrite(P8_7, HIGH);
}

void setMergeLEDLow()
{
  digitalWrite(P8_11,LOW);
}

void setForkLEDLow()
{
  digitalWrite(P8_7,LOW);
}

extern FILE *fp;

/* Overall gradient function for detecting merges and forks */
static int gradient(List_t *list, Data_t *data, int select)
{
  List_t *head;
  float average = 0;
  int i;
  /* Code for left gradient */
  if(select==1)
  {
    head = list;

    for(i=0;i<3;i++)
    {
      /* Sum for an average of the past three distances */
      average+=head->data.frontLeft;

      head = head->prev;
    }

    average = average/3;


    return ((data->frontLeft - average)/average > 1.7);
  }

  /* Code for right gradient */
  if(select==2)
  {
    head = list;

    for(i=0;i<3;i++)
    {
      /*Sum for an average of the past three distances */
      average += head->data.frontRight;

      head = head->prev;
    }

    average = average/3;

    return((data->frontRight - average)/average > 1.7);
  }

  /* Code for left fork portion */
  if(select==3)
  {
    head = list;

    for(i=0;i<3;i++)
    {
      average += head->data.backLeft;

      head = head->prev;
    }

    average = average/3;

    return ((data->backLeft - average)/average > 0.15);
  }

  /* Code for right fork portion */
  if(select==4)
  {
    head = list;

    for(i=0;i<3;i++)
    {
      average += head->data.backRight;
      head = head->prev;
    }

    average = average/3;

    return ((data->backRight - average)/average > 0.15);
  }

  return 0;
}






/* function to determine if the last few states were forks, or merges */
int cooldown(List_t *list)
{
  int i;
  List_t *hold = list;
  for(i=0;i<RINGSIZE;i++)
  {
    if(hold->data.trackState == RIGHTFORK || hold->data.trackState == LEFTFORK ||
       hold->data.trackState == FORK || hold->data.trackState == MERGERIGHT ||
       hold->data.trackState == MERGELEFT)
    {
      return 0;
    }
    hold = hold->next;
  }
  return 1;
}

/* Global indexing variable used for keeping track of if the
   vehicle should go left or right at a fork */
int alternate=0;
extern int turnstate;

void trackDetection(List_t *list, Data_t * data)
{
  /* Look for Y fork */
  if(cooldown(list))
  {
    setForkLEDLow();
    setMergeLEDLow();
  }
    // setCarSpeed(0);
    // setForkLEDHigh();
    // delay(10000);
    // setMergeLEDLow();
    // setCarSpeed(CARSPEED);

  if(gradient(list,data,3) && gradient(list,data,4))
  {
      /* Y fork confirmed */
      // printf("Y fork found\n");
      setForkLEDHigh();
      /* Consult the index */
      if(cooldown(list))
      {
        //setCarSpeed(0);
        if(alternate==0)
        {
          alternate = 1;
          turnstate = FOLLOWRIGHT;
        }
        else
        {
          alternate = 0;
          turnstate = FOLLOWLEFT;
        }
      }

    data->trackState = FORK;
  }

  if(gradient(list,data,2))
  {
    setMergeLEDHigh();

    if(cooldown(list))
    {
      turnstate = FOLLOWLEFT;
    }

    data->trackState = MERGERIGHT;
  }

  if(gradient(list,data,1))
  {
    setMergeLEDHigh();

    if(cooldown(list))
    {
      turnstate = FOLLOWRIGHT;
    }

    data->trackState = MERGELEFT;
  }


  /* Look at the right side for merges and forks */
  // if(isMuchLarger(data->frontRight,data->backRight) && data->frontRight > 33 &&
  //    !isMuchLarger(data->frontLeft,data->backLeft))
  // {
  //   if(checkGradientRight(list,data)&& data->frontRight > 33)
  //   {
  //     /* If the gradient is large, then assume Merge */
  //     // printf("Right Merge\n");
  //     if(cooldown(list)) turnstate = FOLLOWLEFT;
  //
  //     setMergeLEDHigh();
  //     // fp = fopen("forkResults.txt","a+");
  //     // fprintf(fp,"Right merge detected!\n");
  //     // fprintf(fp,"front left = %f, front right = %f\n",data->frontLeft,data->frontRight);
  //     // fprintf(fp,"back left = %f, back right = %f\n\n",data->backLeft,data->backRight);
  //     // fclose(fp);
  //
  //     data->trackState = MERGERIGHT;
  //     // setCarSpeed(0);
  //     // setMergeLEDHigh();
  //     // delay(10000);
  //     // setMergeLEDLow();
  //     // setCarSpeed(CARSPEED);
  //
  //   }
  //   else if(isMuchLarger(list->data.frontRight,list->data.backRight) &&
  //           isRoughlyEqual(list->data.frontLeft,list->data.backLeft) &&
  //           isRoughlyEqual(data->frontLeft,data->backLeft) &&
  //           data->frontLeft < 18 && data->backLeft < 18 &&
  //           data->frontLeft > data->backLeft)
  //   {
  //     /* If the previous set of right distances were also
  //        much larger, then assume a right fork */
  //     // printf("Straight Right Fork\n");
  //     /* Consult the index */
  //     if(cooldown(list))
  //     {
  //       if(alternate==0)
  //       {
  //         alternate = 1;
  //         turnstate = FOLLOWRIGHT;
  //       }
  //       else
  //       {
  //         alternate = 0;
  //         turnstate = FOLLOWLEFT;
  //       }
  //       setForkLEDHigh();
  //     }
  //
  //     // fp = fopen("forkResults.txt","a+");
  //     // fprintf(fp,"Right fork detected!\n");
  //     // fprintf(fp,"front left = %f, front right = %f\n",data->frontLeft,data->frontRight);
  //     // fprintf(fp,"back left = %f, back right = %f\n\n",data->backLeft,data->backRight);
  //     // fclose(fp);
  //
  //
  //     data->trackState = RIGHTFORK;
  //     // setCarSpeed(0);
  //     // setForkLEDHigh();
  //     // delay(10000);
  //     // setForkLEDLow();
  //     // setCarSpeed(CARSPEED);
  //
  //   }
  // }
  // /* Look at the left side for merges or forks */
  // else if(isMuchLarger(data->frontLeft,data->backLeft) && data->frontLeft > 33 &&
  //         !isMuchLarger(data->frontRight,data->backRight))
  // {
  //   if(checkGradientLeft(list,data) && data->frontLeft > 33)
  //   {
  //     /* If the gradient is large, then assume Merge */
  //     // printf("Left Merge\n");
  //     setMergeLEDHigh();
  //     // fp = fopen("forkResults.txt","a+");
  //     // fprintf(fp,"Left merge detected!\n");
  //     // fprintf(fp,"front left = %f, front right = %f\n",data->frontLeft,data->frontRight);
  //     // fprintf(fp,"back left = %f, back right = %f\n\n",data->backLeft,data->backRight);
  //     // fclose(fp);
  //     turnstate = FOLLOWRIGHT;
  //
  //     data->trackState = MERGELEFT;
  //     // setMergeLEDHigh();
  //     // setCarSpeed(0);
  //     // delay(10000);
  //     // setMergeLEDLow();
  //     // setCarSpeed(CARSPEED);
  //
  //   }
  //   else if(isMuchLarger(list->data.frontLeft,list->data.backLeft) &&
  //           isRoughlyEqual(list->data.frontRight,list->data.backRight) &&
  //           isRoughlyEqual(data->frontRight,data->backRight) &&
  //           data->frontRight < 18 && data->frontRight < 18 &&
  //           data->frontRight > data->backRight)
  //   {
  //     /* If the previous set of right distances were also
  //        much larger, then assume a left fork */
  //     // printf("Straight Left Fork\n");
  //     /* Consult the index */
  //     if(cooldown(list))
  //     {
  //       if(alternate==0)
  //       {
  //         alternate = 1;
  //         turnstate = FOLLOWRIGHT;
  //       }
  //       else
  //       {
  //         alternate = 0;
  //         turnstate = FOLLOWLEFT;
  //       }
  //       data->trackState = LEFTFORK;
  //     }
  //
  //
  //     // fp = fopen("forkResults.txt","a+");
  //     // fprintf(fp,"Left fork detected!\n");
  //     // fprintf(fp,"front left = %f, front right = %f\n",data->frontLeft,data->frontRight);
  //     // fprintf(fp,"back left = %f, back right = %f\n\n",data->backLeft,data->backRight);
  //     // fclose(fp);
  //
  //       setForkLEDHigh();
  //
  //     // setForkLEDHigh();
  //     // setCarSpeed(0);
  //     // delay(10000);
  //     // setForkLEDLow();
  //     // setCarSpeed(CARSPEED);
  //
  //   }
  // }


}


// void trackFeatureDetection(Data_t * data)
// {
//   float forwardRight = data->frontRight;
//   float forwardLeft = data->frontLeft;
//   float backRight = data->backRight;
//   float backLeft = data->backLeft;
//
//   /* If the forward sensors see more than two cm
//      difference in distance from the back sensors
//      then there is a fork. Return 2 for fork */
//   if(forwardRight > backRight+5 && forwardLeft > backLeft+5){
//     data->trackState = FORK;
//   }
//   /* If all distances are equal aside from the front
//      left distance, then the car is at a left hand
//      straight fork */
//   else if(isRoughlyEqual(forwardRight,backRight) && forwardLeft > forwardRight+15){
//     return STRAIGHTFORKLEFT;
//   }
//   /* If all distances are equal aside from the front
//      right, then the car is at a right hand straight
//      fork */
//   else if(isRoughlyEqual(forwardLeft,backLeft) && forwardRight > forwardLeft+15){
//     return STRAIGHTFORKRIGHT;
//   }
//   /* If the forward left sensor sees a distance much
//      larger than the forward right distance, then the
//      car is at a left hand merge */
//   else if(forwardLeft > forwardRight+25){
//     return MERGELEFT;
//   }
//   /* If the forward right sensor sees a distance much
//      larger than the forward left distance, then the
//      car is at a right hand merge */
//   else if(forwardRight > forwardLeft+25){
//     return MERGERIGHT;
//   }
//
//
//   return DEFAULT;
// }
