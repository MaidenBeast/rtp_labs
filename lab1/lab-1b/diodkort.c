#include <unistd.h>
#include <sys/io.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

/* Port 0 (0x180) is the first set of leds, Port 1 second set of leds and Port 2 third set of leds as well as inputs from the switches. */

/* Attempts to sleep for a specified number of micro-seconds */
void busyWait(int useconds) {
  struct timeval tv1, tv2;
  gettimeofday(&tv1,NULL);
  do {
    gettimeofday(&tv2,NULL);
  } while((tv2.tv_sec-tv1.tv_sec)*1000000+(tv2.tv_usec-tv1.tv_usec) < useconds);
}

void shine(unsigned int lightIntensity) { //lightIntensity: percentage (from 0 to 99) 
	unsigned int shinePeriod;
	
	//	if the percent is greater than 100% --> set the shine period to 100%
	//	(for avoid strange interval overflows) 
	shinePeriod = (lightIntensity < 100) ? 20000/100*lightIntensity : 20000;
	
	outb(0xFF, 0x180);	//turn on all LEDs of the first port
	outb(0XFF, 0x181);	//turn on all LEDs of the second port
	busyWait(shinePeriod); //delay for the shinePeriod
	outb(0x00, 0x180); //turn off all LEDs of the first port
	outb(0x00, 0x181); //turn off all LEDs of the second port
	busyWait(20000 - shinePeriod); //delay for the remaining time until the 20000th ms
}

int chkTurnedOnSwitches(char switches) {
	int zero_values = 0;
	
	do {
		if (switches & (unsigned int)0x80)
			zero_values++;
		switches <<= 1;
	} while (switches > (unsigned int)0);
	
	return zero_values;
}


int main() {
	unsigned int counter = 0;
	unsigned int intensity = 0;

  /* Ask OS for permission to manipulate the I/O ports */
  if(ioperm(0x180, 5, 1)) {
    fprintf(stderr,"Failed to get I/O permissions to card, you must run as sudo\n");
    exit(0);
  }
  
  /* Set 1 to register 0x184 to enable outputs from the card */
  outb(1, 0x184);
  outb(0xFF,0x181);
  
  outb(0xFF, 0x182); //enable reading switches status

  while(1) {
	//sample-code (uncomment for test it)
    // /* Turn on four of the LED's */
    // outb(0xCC,0x180);
    // sleep(1);
    // /* Turn them off and turn on the other four */
    // outb(0x33,0x180);
    // sleep(1);
	
	//shine(50); //50% of shine intensity <-- point 1 (without counter)
	
	//point 2 (with counter)
	/*shine(intensity);
	counter++;
	intensity = (counter)%100;*/
	
	//point 3 (regulated by switches)
	char switches = inb(0x182); //read the switches status
	int turnedOnSwitches = chkTurnedOnSwitches(switches); //how many switches are turned on
	intensity = turnedOnSwitches*100/8;
	shine(intensity);
  }
}
