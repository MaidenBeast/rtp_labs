#include "common.h"
#include "fan.h"
#include "lamp.h"
#include "motor1.h"

void motor1() {

	motor1_direction = CLOCKWISE;
	motor1_speed = LOW_SPEED;

	char msg[2];

	while(1) {
		msgQReceive(m1MsgQId, msg, 2, WAIT_FOREVER);

		if (strcmp(msg, "1")==0) {
			rotateMotor1(FULL_ROTATION_STEPS);
		} else if (strcmp(msg, "2")==0) {
			rotateMotor1(HALF_ROTATION_STEPS);
		} else if (strcmp(msg, "3")==0) {
			//Change direction of rotations for tool 1.
			motor1_direction = (motor1_direction==CLOCKWISE) ? COUNTERCLOCKWISE : CLOCKWISE;
		} else if (strcmp(msg, "a")==0) {
			//Change the speed of rotations for tool 1.
			motor1_speed = (motor1_speed==LOW_SPEED) ? HIGH_SPEED : LOW_SPEED;
		}

		taskDelay(10);
	}
}

void rotateMotor1(int n_rotations) {
	int ticks_interval = (motor1_speed==HIGH_SPEED) ? sysClkRateGet()/60 : sysClkRateGet()/10;

	//Rotate tool 1 one full rotation.
	char dir = (motor1_direction==COUNTERCLOCKWISE) ? M1_DIR : 0x00;

	changeLampMode(MACHINE_WORKING);
	changeFanMode(FAN_ONE_HUNDRED_PERCENT);
	
	semTake(semMotors, WAIT_FOREVER);
	for (counter_motor1_steps=0; counter_motor1_steps<n_rotations; counter_motor1_steps++) {
		sysOutByte(0x181,M2_INHIB|M1_STEP|M1_HFM|dir);
		taskDelay(5);
		sysOutByte(0x181,M2_INHIB|dir);
		taskDelay(ticks_interval-5);
	}
	taskDelay(1000);
	semGive(semMotors);

	changeLampMode(MACHINE_NOT_WORKING);
	changeFanMode(FAN_FIFTY_PERCENT);
}
