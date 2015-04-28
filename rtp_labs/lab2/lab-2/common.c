#include "vxWorks.h"

//checks if a pulse if detected from the reflex detector
int checkTrigger() {
	char byte_in = sysInByte(0x183);
	return ((byte_in & 0x01) == 0x00); //returns true if the most-right bit is equal 0
}
