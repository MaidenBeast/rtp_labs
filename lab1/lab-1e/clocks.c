/* clocks.c
 * Utför några tester på hur klockan fungerar med olika hastigheter 
 */

#include <stdio.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include "math.h"

double timenow() {
  struct timespec tv;
  clock_gettime(CLOCK_REALTIME,&tv);
  return tv.tv_sec + tv.tv_nsec * 1e-9;
}
void delay(double dt) {
  struct timespec tv;
  tv.tv_sec = (int) dt;
  tv.tv_nsec = fmod(dt,1.0) * 1000000000;
  nanosleep(&tv,NULL);
}

void getres() {
  struct timespec tv;
  clock_getres(CLOCK_REALTIME,&tv);
  printf("Clock resolution is: %.6f",tv.tv_sec+1e-9*tv.tv_nsec);
}

void clockTest() {
  double startTime = timenow();
  double nextTime = startTime;
  double previousTime = -1.0; //negative time, i.e. "there is no previous time"
  double deltaTime;
  char *syncResultStr;
  
  while(1) {
	if (previousTime == -1.0) { //if there is no previous time
		printf("%3.6f\n",timenow() - startTime); //print only the actual relative time
	} else {
		//output string check of 7Hz clock runtime 
		syncResultStr = (deltaTime == 1/7.0) ? "Sync to 7Hz" : "Not sync to 7Hz";
		
		// print relative time, delta time, and if the clock is "sync" to 7Hz
		printf("%3.6f\t %3.6f\t %s\n",timenow() - startTime, deltaTime, syncResultStr);
	}
	printf("Clock frequency: %d\n", sysClkRateGet());
	
    previousTime = timenow() - startTime;
    
    nextTime += 1/7.0;
    delay(nextTime - timenow());
    //delta between previous and actual time
    deltaTime = timenow() - startTime - previousTime;
  }
}
