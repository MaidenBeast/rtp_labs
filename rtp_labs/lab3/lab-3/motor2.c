#include "common.h"
#include "fan.h"
#include "lamp.h"
#include "motor2.h"

void motor2() {
	int counter_motor2_steps;		//48 full-steps per rotation

	motor2_direction = CLOCKWISE;
	motor2_speed = LOW_SPEED;

	char msg[2];

	while(1) {
		msgQReceive(m2MsgQId, msg, 2, WAIT_FOREVER);

		if (strcmp(msg, "4")==0) {
			int ticks_interval = (motor2_speed==HIGH_SPEED) ? sysClkRateGet()/60 : sysClkRateGet()/10;
			
			//Rotate tool 2 one full rotation.
			
			char dir = (motor2_direction==COUNTERCLOCKWISE) ? M2_DIR : 0x00;
			
			for (counter_motor2_steps=0; counter_motor2_steps<FULL_ROTATION_STEPS; counter_motor2_steps++) {
				sysOutByte(0x181,M1_INHIB|M2_STEP|M2_HFM|dir);
				taskDelay(5);
				sysOutByte(0x181,M1_INHIB|dir);
				taskDelay(ticks_interval-5);
			}

		} else if (strcmp(msg, "5")==0) {
			int ticks_interval = (motor2_speed==HIGH_SPEED) ? sysClkRateGet()/60 : sysClkRateGet()/10; 
			
			//Rotate tool 1 one half rotation.
			
			char dir = (motor2_direction==COUNTERCLOCKWISE) ? M2_DIR : 0x00;
			
			for (counter_motor2_steps=0; counter_motor2_steps<HALF_ROTATION_STEPS; counter_motor2_steps++) {
				sysOutByte(0x181,M1_INHIB|M2_STEP|M2_HFM|dir);
				taskDelay(5);
				sysOutByte(0x181,M1_INHIB|dir);
				taskDelay(ticks_interval-5);
			}

			counter_motor2_steps = 0;

		} else if (strcmp(msg, "6")==0) {
			//Change direction of rotations for tool 1.
			motor2_direction = (motor2_direction==CLOCKWISE) ? COUNTERCLOCKWISE : CLOCKWISE;
		} else if (strcmp(msg, "b")==0) {
			//Change the speed of rotations for tool 1.
			motor2_speed = (motor2_speed==LOW_SPEED) ? HIGH_SPEED : LOW_SPEED;
		}

		taskDelay(10);
	}
}
