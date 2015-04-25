#include "vxWorks.h"

int checkTrigger() {
	char byte_in = sysInByte(0x183);
	return ((byte_in & 0x01) == 0x00);
}
