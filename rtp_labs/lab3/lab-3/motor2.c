#include "common.h"
#include "fan.h"
#include "lamp.h"
#include "motor2.h"
#include "motor1.h"

void motor2() {
	motor2_waiting = 0;
	
	int counter_motor2_steps;		//48 full-steps per rotation

	motor2_direction = CLOCKWISE;
	motor2_speed = LOW_SPEED;

	char msg[2];

	while(1) {
		msgQReceive(m2MsgQId, msg, 2, WAIT_FOREVER);

		if (strcmp(msg, "4")==0) {
			rotateMotor2(FULL_ROTATION_STEPS);
		} else if (strcmp(msg, "5")==0) {
			rotateMotor2(HALF_ROTATION_STEPS);
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

void rotateMotor2(int n_rotations) {
	int ticks_interval = (motor2_speed==HIGH_SPEED) ? sysClkRateGet()/60 : sysClkRateGet()/10;

	//Rotate tool 1 one full rotation.
	char dir = (motor2_direction==COUNTERCLOCKWISE) ? M2_DIR : 0x00;

	motor2_waiting = 1;

	semTake(semMotors, WAIT_FOREVER);

	changeLampMode(MACHINE_WORKING);
	changeFanMode(FAN_ONE_HUNDRED_PERCENT);

	for (counter_motor2_steps=0; counter_motor2_steps<n_rotations; counter_motor2_steps++) {
		sysOutByte(0x181,M2_STEP|M2_HFM|dir);
		taskDelay(5);
		sysOutByte(0x181,dir);
		taskDelay(ticks_interval-5);
	}

	if (motor1_waiting) {
		taskDelay(1000);
	}

	changeLampMode(MACHINE_NOT_WORKING);
	changeFanMode(FAN_FIFTY_PERCENT);

	semGive(semMotors);

	motor2_waiting = 0;
}
