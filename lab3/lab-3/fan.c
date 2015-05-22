#include "common.h"
#include "fan.h"

//int fan_counter = 0;

void fan() {
	while (1) {
		switch (fan_mode) {
		case FAN_OFF:
			//only if fan was previously on
			if (fan_state) {
				fan_state = 0;
				sysOutByte(0x182, (lamp_state) ? LAMP_FLAG : 0x00); //set low the fan flag
			}
			taskDelay(10);
			break;
		case FAN_FIFTY_PERCENT: 		//100 Hz --> 10 ms for each cycle
			sysOutByte(0x182, (lamp_state) ? FAN_FLAG|LAMP_FLAG : FAN_FLAG); //turn on the fan
			fan_state = 1;
			//delayMsec(5);
			taskDelay(5);	//5 ms of turned on fan
			sysOutByte(0x182, (lamp_state) ? LAMP_FLAG : 0x00);	//turn off the fan
			fan_state = 0;
			//delayMsec(5);
			taskDelay(5);	//5 ms of turned off fan
			break;
		case FAN_ONE_HUNDRED_PERCENT:
			//only if fan was previously off
			if (!fan_state) {
				fan_state = 1;
				sysOutByte(0x182, (lamp_state) ? FAN_FLAG|LAMP_FLAG : FAN_FLAG); //set high the fan flag
			}
			taskDelay(10);
			break;
		}
	}
}

void changeFanMode(fan_mode_t mode) {
	//fan_counter = 0;
	fan_mode = mode;
}


//START TEST FUNCTION
void start_test_fan() {
	sysClkRateSet(1000);
	kernelTimeSlice(10);
	fan_state = 0;
	sysOutByte(0x184, 0xFF);
	fan_PID = taskSpawn("fan", 200, 0, 1000, fan);
}

//STOP TEST FUNCTION
void stop_test_fan() {
	taskDelete(fan_PID);
}
