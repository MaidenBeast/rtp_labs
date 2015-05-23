#include "common.h"
#include "fan.h"
#include "lamp.h"
#include "motor1.h"
#include "motor2.h"

void motor1() {
	motor1_waiting = 0;

	motor1_direction = CLOCKWISE;
	motor1_speed = LOW_SPEED;

	char msg[2];

	while(1) {
		msgQReceive(m1MsgQId, msg, 2, WAIT_FOREVER);

		if (strcmp(msg, "1")==0) {				//"Rotate tool 1 one full rotation."
			rotateMotor1(FULL_ROTATION_STEPS);	
		} else if (strcmp(msg, "2")==0) {		//"Rotate tool 1 one half rotation."
			rotateMotor1(HALF_ROTATION_STEPS);
		} else if (strcmp(msg, "3")==0) {		//"Change direction of rotations for tool 1."
			motor1_direction = (motor1_direction==CLOCKWISE) ? COUNTERCLOCKWISE : CLOCKWISE;
		} else if (strcmp(msg, "a")==0) {		//"Change the speed of rotations for tool 1."
			motor1_speed = (motor1_speed==LOW_SPEED) ? HIGH_SPEED : LOW_SPEED;
		}

		taskDelay(10);
	}
}

void rotateMotor1(int n_rotations) {
	//one tick interval correspond to the time interval dedicated for one step
	//HIGH_SPEED: 	60 steps per second --> 1000/60 ms
	//LOW_SPEED: 	10 steps per second --> 1000/10 ms
	int tickMs = 1000/sysClkRateGet();

	int ticks_interval = (motor1_speed==HIGH_SPEED) ? sysClkRateGet()/60 : sysClkRateGet()/10;
	//int ticks_interval = (motor1_speed==HIGH_SPEED) ? sysClkRateGet()/(60*tickMs) : sysClkRateGet()/(10*tickMs); //TO TEST

	//Set the direction flag depending from the motor1_direction value
	// M1_DIR high --> COUNTERCLOCKWISE
	// M1_DIR low  --> CLOCKWISE
	char dir = (motor1_direction==COUNTERCLOCKWISE) ? M1_DIR : 0x00;

	motor1_waiting = 1;

	semTake(semMotors, WAIT_FOREVER); 			//"Under no circumstance are the machines allowed to run at the same time"

	changeLampMode(MACHINE_WORKING);			//"When the machines are working, the lamp should blink with 3 on/off flashes every second."
	changeFanMode(FAN_ONE_HUNDRED_PERCENT);		//"When the machines are operating, the fan should be working at 100%"

	counter_motor1_steps = n_rotations;
	
	//decremental conting. In this way, for stopping the motors at the next safe position,
	//it is enough to decrement (elsewhere) the counter
	while (counter_motor1_steps > 0) {
		semTake(semCounterMotor1, WAIT_FOREVER);		//take the semaphore, in this way nothing else can access to the counter
		
		//negative edge, for stepping the motor 1.
		sysOutByte(0x181,M1_STEP|M1_HFM|dir);
		taskDelay(5);					//5 ms of delay
		sysOutByte(0x181,dir);
		taskDelay(ticks_interval-5);	//delay for respecting the stepping speed (less of the previous 5 ms delay)
		counter_motor1_steps--;			//decrement the counter
		
		semGive(semCounterMotor1);
	}
	
	/*for (counter_motor1_steps=0; counter_motor1_steps<n_rotations; counter_motor1_steps++) {
		sysOutByte(0x181,M1_STEP|M1_HFM|dir);
		taskDelay(5);
		sysOutByte(0x181,dir);
		taskDelay(ticks_interval-5);
	}*/
	
	sysOutByte(0x181, 0x00);

	if (motor2_waiting) {					//"...This margin should not be applied when starting a second job on the same motor"
		taskDelay(1000);					//"there must always be a safety margin of 1s from one machine stopping before the next can start ..."
	}

	changeLampMode(MACHINE_NOT_WORKING);	//"When the machines are not, the lamp should smoothly go from zero intensity to full intensity, and then smoothly back to zero"
	changeFanMode(FAN_FIFTY_PERCENT);		//"...otherwise the fan should be working at 50%"
	
	motor1_waiting = 0;

	semGive(semMotors);

}
