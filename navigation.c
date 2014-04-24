/*
* navigation.c
*
* Holds the functions for navigating through merges, forks, and turns
*
* Author: Marshall Newberry
* Date: 8/2/2015
*/

#include "wall_following.h"
#include "navigation.h"


#define RIGHT1 1
#define RIGHT2 2
#define LEFT1  3
#define LEFT2  4
#define FIRST  1
#define SECOND 2
#define THIRD  3
#define STRAIGHTRIGHT 1
#define STRAIGHTLEFT  2
#define TURNRIGHT     3
#define TURNLEFT      4

/*
* void analyze_thread(void)
*
* Handles the main navigation and calls the proper wall following algorithm.
* Uses nested ifelse statements to determine what track feature the vehicle is
* at.
* When all four distances are equal, the vehicle should continue straight
* and use the straight wall following algorithm.
* When the front two distances are skewed and the back two are equal, the
* track is turning in the direction of the larger distance. Use the turn
* wall following algorithm to follow the outside wall.
* When the back two distances and one of the forward distances are equal
* while the other forward distance gradually falls out, the track is at
* a straight fork. Consult the turn index and turn or go straight accordingly.
* When the forward distances are equal but larger than the equal rear distances,
* the track is at a Y fork. Consult the turn index and turn left or right
* accordingly.
* When one side drops out suddenly (no gradual change) the track is merging,
* follow the side that didn't drop out and continue with the previous wall
* following algorithm.
*
* Takes no inputs, reads distances from global position data.
*/

int position[4][3];
int follow_direction;

#define STRAIGHTFORK 1
#define STRAIGHT 2
#define TURNRIGHT 3
#define TURNLEFT 4
#define YFORK 5
#define STRAIGHTMERGE 6
#define YMERGE 7
void analyze_thread(void){
  int track_state, index, right1, left1, right2, left2;

  /* Straight */
  if(track_state == STRAIGHT){
    if(left2<=10 && right2>=20){
      track_state = TURNRIGHT;
      follow_direction = TURNRIGHT;
    }
    else if(left2>=20 && right2<=10){
      track_state = TURNLEFT;
      follow_direction = TURNLEFT;
    }
    else if(left2>=20 && right2>=20){
      track_state = YFORK;
      /* Consult turn index */
      if(index==1) follow_direction = TURNRIGHT;
      else if(index==0) follow_direction = TURNLEFT;
    }
    else if(right2>=20 && 14<=left2<=16){
      /* Consult turn index */
      if(index==1) follow_direction = TURNRIGHT;
      else if(index==0) follow_direction = STRAIGHTLEFT;
    }
    else if(14<=right2<=16 && left<20){
      track_state = STRAIGHTMERGE;
      follow_direction = STRAIGHTRIGHT;
    }
  }

  else if(track_state = TURNRIGHT){
    if(-1<=(right1-right2)<=1){
      track_state = STRAIGHT;
      follow_direction = STRAIGHTRIGHT;
    }
  }

  else if(track_state = TURNLEFT){
    if(-1<=(left1-left2)<=1){
      track_state = STRAIGHT;
      follow_direction = STRAIGHTLEFT;
    }
  }

}
