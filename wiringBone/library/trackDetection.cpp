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

#include "trackDetection.h"
#include <math.h>


/* Absolute value function since math.h didn't seem to include one */
static float abs(float number)
{
  if(number<0) return -number;
  else return number;
}

/* Maximum float function */
float maximum(float right, float left)
{
  if(right<left) return left;
  else return right;
}

/* Minimum float function */
static int isRoughlyEqual(float left, float right){
  if(abs(left-right)/maximum(left,right) < 0.3) return 1;
  else return 0;
}

/* Function for determining if a number is much greater (40%)
   than the other. Returns true if the first number is much
   greater than the second */
static int isMuchLarger(float one, float two)
{
  /* If one is not greater than two
     it is not much larger than two */
  if(!(one>two))
  {
    return 0;
  }
  /* If the difference between the two divided by
     the maximum of the two is less than a threshold
     then one is not much larger than two */
  else if((one-two)/maximum(one,two) < 0.25)
  {
    return 0;
  }
  /* All checks have been passed, one must be
     larger than two */
  return 1;
}


/* Function for checking the gradient on the left side.
   If the difference between the front and back distance
   now is much larger than it was before, return true */
static int checkGradientLeft(List_t *list, Data_t *data)
{
  float currentDifference, previousDifference;
  currentDifference = data->frontLeft - data->backLeft;
  previousDifference = list->data.frontLeft - list->data.backLeft;
  return isMuchLarger(currentDifference,previousDifference);
}

/* Function for checking the gradient on the right side.
   If the difference between the front and back distance
   now is much larger than it was before, return true */
static int checkGradientRight(List_t *list, Data_t *data)
{
  float currentDifference, previousDifference;
  currentDifference = data->frontRight - data->backRight;
  previousDifference = list->data.frontRight - list->data.backRight;
  return isMuchLarger(currentDifference,previousDifference);
}

/* Global indexing variable used for keeping track of if the
   vehicle should go left or right at a fork */
int index=0;


void trackDetection(List_t *list, Data_t * data, int *cooldown)
{
  /* if the cooldown count is still high, decrement and
  maintain the current track state */
  if(*cooldown>0)
  {
    *cooldown -= 1;
    printf("cooldown decrement: %d\n",*cooldown);
  }
  else
  {
    if(isMuchLarger(data->frontRight,data->backRight) &&
       isMuchLarger(data->frontLeft,data->backLeft))
    {
      /* Y fork checking */
      if(isMuchLarger(list->data.frontRight,list->data.backRight) &&
         isMuchLarger(list->data.frontLeft,list->data.backLeft))
      {
        /* Y fork confirmed */
        printf("Y fork found\n");
        /* Consult the index */
        if(index==0)
        {
          index = 1;
          data->trackState = FOLLOWRIGHT;
        }
        else
        {
          index = 0;
          data->trackState = FOLLOWLEFT;
        }

        /* Set cooldown */
        *cooldown = COOLDOWN;
        printf("cooldown set\n");
      }
    }
    else if(isMuchLarger(data->frontRight,data->backRight))
    {
      if(checkGradientRight(list,data))
      {
        /* If the gradient is large, then assume Merge */
        printf("Right Merge\n");
        data->trackState = FOLLOWLEFT;
        *cooldown = COOLDOWN;
        printf("cooldown set\n");
      }
      else if(isMuchLarger(list->data.frontRight,list->data.backRight))
      {
        /* If the previous set of right distances were also
           much larger, then assume a right fork */
        printf("Straight Right Fork\n");
        /* Consult the index */
        if(index==0)
        {
          index = 1;
          data->trackState = FOLLOWRIGHT;
        }
        else
        {
          index = 0;
          data->trackState = FOLLOWLEFT;
        }

        /* Set cooldown */
        *cooldown = COOLDOWN;
        printf("cooldown set\n");
      }
    }
    else if(isMuchLarger(data->frontLeft,data->backLeft))
    {
      if(checkGradientLeft(list,data))
      {
        /* If the gradient is large, then assume Merge */
        printf("Left Merge\n");
        data->trackState = FOLLOWRIGHT;
        *cooldown = COOLDOWN;
        printf("cooldown set\n");
      }
      else if(isMuchLarger(list->data.frontLeft,list->data.backLeft))
      {
        /* If the previous set of right distances were also
           much larger, then assume a left fork */
        printf("Straight Left Fork\n");
        /* Consult the index */
        if(index==0)
        {
          index = 1;
          data->trackState = FOLLOWRIGHT;
        }
        else
        {
          index = 0;
          data->trackState = FOLLOWLEFT;
        }

          /* Set cooldown */
          *cooldown = COOLDOWN;
          printf("cooldown set\n");

      }
    }
  }
  printf("Track Detection Complete");
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
