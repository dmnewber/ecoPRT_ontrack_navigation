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

static int isRoughlyEqual(float left, float right){
  if(abs(left-right)/max(left,right) < 0.1) return 1;
  else return 0;
}

int trackFeatureDetection(float forwardRight, float forwardLeft,
                          float backRight, float backLeft){
  /* If the forward sensors see more than two cm
     difference in distance from the back sensors
     then there is a fork. Return 2 for fork */
  if(forwardRight > backRight+7 && forwardLeft > backLeft+7){
    return FORK;
  }
  /* If all distances are equal aside from the front
     left distance, then the car is at a left hand
     straight fork */
  else if(isRoughlyEqual(forwardRight,backLeft) && isRoughlyEqual(backLeft,backRight)
          && forwardLeft > forwardRight+7){
    return STRAIGHTFORKLEFT;
  }
  /* If all distances are equal aside from the front
     right, then the car is at a right hand straight
     fork */
  else if(isRoughlyEqual(forwardLeft,backRight) && isRoughlyEqual(backRight,backLeft)
          && forwardRight > forwardLeft+7){
    return STRAIGHTFORKRIGHT;
  }
  /* If the forward left sensor sees a distance much
     larger than the forward right distance, then the
     car is at a left hand merge */
  else if(forwardLeft > forwardRight+20){
    return MERGELEFT;
  }
  /* If the forward right sensor sees a distance much
     larger than the forward left distance, then the
     car is at a right hand merge */
  else if(forwardRight > forwardLeft+20){
    return MERGERIGHT;
  }


  return DEFAULT;
}
