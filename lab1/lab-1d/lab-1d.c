/* Ovn2 - A simple program for an automatic plug-saw */

#include "vxWorks.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* It represents a point which saw must go through */
struct point {
  float x;
  float y;
};

/* 	Call using successive points to be cut, print, total
	number of points and the route that has been sawn */
void cutContour(struct point *newPoint) {
  static float totalLength=0.0;
  static struct point lastPoint;
  static int nPoints=0;
  float deltaX,deltaY,length;

  if(nPoints == 0) {
    nPoints = 1;
    lastPoint = *newPoint;
    printf("Starting from point: (%3.1f, %3.1f)\n",newPoint->x,newPoint->y);
    return;
  } else {

    deltaX = newPoint->x - lastPoint.x;
    deltaY = newPoint->y - lastPoint.y;
    length=sqrt(deltaX*deltaX+deltaY*deltaY);
    totalLength += length;

    nPoints++;
    lastPoint = *newPoint;    
    printf("Cutting to point: (%3.1f, %3.1f)\n",newPoint->x,newPoint->y);
    printf("Total points: %d\n",nPoints);
    printf("Total length: %3.1f\n",totalLength);
    return;
  }
}

/* 	Prompts for points that story and call cutContour with points.
	Cancel by providing new row instead of entering the next point. */
void doInput() {
  char str[256], *pntr, *pntr2;
  struct point inputPoint;

  for(;;) {
    printf("Ange X och Y koordinat: "); 
    fflush(stdout);
    gets(str);
    if(str[0] == '\n') break;

    inputPoint.x = (float)strtod(str,&pntr);
    if(pntr == str) {
      printf("Kunde inte lasa nagot tal. \nForsok igen med exempelvis: 0.0 0.0\n");
      continue;
    }
    inputPoint.y = (float)strtod(pntr,&pntr2);
    if(pntr == pntr2) {
      printf("Kunde bara lasa in det forsta talet. \nForsok igen med exempelvis: 0.0 0.0\n");
      continue;
    }
    cutContour(&inputPoint);
  }
}
