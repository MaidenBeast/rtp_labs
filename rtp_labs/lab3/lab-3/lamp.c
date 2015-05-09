#include "common.h"
#include "lamp.h"

int lamp_counter = 0;
int lamp_intensity = 0;

void shineLamp(unsigned int period_usec, unsigned int lightIntensity) {
	unsigned int shinePeriod;

	/*
	 * if the percent is greater than 100% --> set the shine period to 100%
	 * (for avoid strange interval overflows) 
	 */
	shinePeriod = (lightIntensity < 100) ? period_usec/100*lightIntensity : period_usec;

	sysOutByte(0x182, (fan_state) ? FAN_FLAG|LAMP_FLAG : LAMP_FLAG); //turn on the lamp
	lamp_state = 1;
	delayUsec(shinePeriod); 				/* delay for the shinePeriod 							*/
	sysOutByte(0x182, (fan_state) ? FAN_FLAG : 0x00);	//turn off the lamp
	lamp_state = 0;
	delayUsec(period_usec - shinePeriod); 		/* delay for the remaining time	*/
}

void changeLampMode(lamp_mode_t mode) {
	lamp_counter = 0;
	lamp_intensity = 0;
	lamp_mode = mode;
}

void lamp() {
	while (1) {
		switch (lamp_mode) {
		case LAMP_OFF:
			if (lamp_state) { //turn off the lamp only if it is still on
				lamp_state = 0;
				sysOutByte(0x182, (fan_state) ? FAN_FLAG : 0x00);
				//sysOutByte(0x182, (fan_state) ? FAN_FLAG|LAMP_FLAG : LAMP_FLAG);
			}
			break;
		case LAMP_CONFIG:
			shineLamp(20000, lamp_intensity); //shine interval for 20ms (2 seconds / 100)
			lamp_counter++;
			lamp_intensity = (lamp_counter)%100;
			break;
		case MACHINE_WORKING:
			sysOutByte(0x182, (fan_state) ? FAN_FLAG|LAMP_FLAG : LAMP_FLAG); //turn on the lamp
			lamp_state = 1;
			delayMsec(166);
			sysOutByte(0x182, (fan_state) ? FAN_FLAG : 0x00);	//turn off the lamp
			lamp_state = 0;
			delayMsec(166);
			break;
		case MACHINE_NOT_WORKING:
			shineLamp(20000, lamp_intensity); //shine interval for 20ms (2 seconds / 100)
			lamp_counter++;
			lamp_intensity = (lamp_counter)%200;
			if (lamp_intensity>100) {
				lamp_intensity = 200 - lamp_intensity;
			}
			break;
		case LAMP_ERR:
			sysOutByte(0x182, (fan_state) ? FAN_FLAG|LAMP_FLAG : LAMP_FLAG); //turn on the lamp
			lamp_state = 1;
			delayMsec(1000);
			sysOutByte(0x182, (fan_state) ? FAN_FLAG : 0x00);	//turn off the lamp
			lamp_state = 0;
			delayMsec(1000);
			break;
		}
	}
}
