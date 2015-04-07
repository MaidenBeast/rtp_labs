#include "delayLib.h"
#include "vxWorks.h"

int diodcardPid;

void shine(unsigned int lightIntensity) { //lightIntensity: percentage (from 0 to 99) 
	unsigned int shinePeriod;
	
	//	if the percent is greater than 100% --> set the shine period to 100%
	//	(for avoid strange interval overflows) 
	shinePeriod = (lightIntensity < 100) ? 20000/100*lightIntensity : 20000;
	
	sysOutByte(0x180, 0xff); //turn on all LEDs of the first port
	sysOutByte(0x181, 0xff); //turn on all LEDs of the second port
	delayUsec(shinePeriod); //delay for the shinePeriod
	sysOutByte(0x180, 0x00); //turn off all LEDs of the first port
	sysOutByte(0x181, 0x00); //turn off all LEDs of the second port
	delayUsec(20000 - shinePeriod); //delay for the remaining time until the 20000th ms
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

void diodcard() {
	sysOutByte(0x184, 0x01); //activate outputs
	unsigned int counter = 0;
	unsigned int intensity = 0;
	sysOutByte(0x182, 0xff); //enable reading switches status
	
	while (1) {
		//shine(50); //50% of shine intensity <-- point 5 (without counter)
		
		//point 5 (with counter)
		/*shine(intensity);
		counter++;
		intensity = (counter)%100;*/
		
		//point 6 (regulated by switches)
		char switches = sysInByte(0x182); //read the switches status
		int turnedOnSwitches = chkTurnedOnSwitches(switches); //how many switches are turned on
		intensity = turnedOnSwitches*100/8;
		shine(intensity);
	}
}

int start() {
	diodcardPid = taskSpawn("diodcard", 200, 0, 1000, diodcard); //priority of 200
	return diodcardPid;
}

int stop() {
	return taskDelete(diodcardPid);
}
