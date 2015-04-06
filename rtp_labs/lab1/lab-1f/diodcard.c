#include "delayLib.h"

void diodcard() {
	sysOutByte(0x184, 0x01); //activate outputs
	
	while (1) {
		sysOutByte(0x180, 0xff); //turn on all LEDs of the first port
		delayUsec(10000); //delay for 10,000 microseconds
		sysOutByte(0x180, 0x00); //turn off all LEDs of the first port
		delayUsec(10000); //delay for 10,000 microseconds
	}
}
