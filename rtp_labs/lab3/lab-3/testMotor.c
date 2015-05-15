#include "common.h"

int testMotor1PID;
int testMotor2PID;

void testMotor1() {
	sysClkRateSet(1000);

	for(;;) {
		int delay = sysClkRateGet()*200/1000;
		//M2_INHIB: motor2 turned off
		//M1_STEP: one step of motor 1
		//M1_DIR: counterclockwise (in case of 0 --> clockwise)
		sysOutByte(0x181,M2_INHIB|M1_STEP|M1_DIR);
		taskDelay(5);
		sysOutByte(0x181,M2_INHIB|M1_DIR);
		taskDelay(delay-5);

		sysOutByte(0x181,M2_INHIB|M1_STEP|M1_HFM|M1_DIR);
		taskDelay(5);
		sysOutByte(0x181,M2_INHIB|M1_DIR);
		taskDelay(delay-5);
	}
}

void testMotor2() {
	sysClkRateSet(1000);

	for(;;) {
		int delay = sysClkRateGet()*200/1000;
		//M2_INHIB: motor2 turned off
		//M1_STEP: one step of motor 1
		//M1_DIR: counterclockwise (in case of 0 --> clockwise)
		sysOutByte(0x181,M1_INHIB|M2_STEP|M2_DIR);
		taskDelay(delay);
		sysOutByte(0x181,M1_INHIB|M2_DIR);
		taskDelay(delay);

		sysOutByte(0x181,M1_INHIB|M2_STEP|M2_HFM|M2_DIR);
		taskDelay(delay);
		sysOutByte(0x181,M1_INHIB|M2_DIR);
		taskDelay(delay);
	}
}

void start_test_motor1() {
	testMotor1PID = taskSpawn("testMotor1", 200, 0, 1000, testMotor1);
}

void start_test_motor2() {
	testMotor2PID = taskSpawn("testMotor2", 200, 0, 1000, testMotor2);
}

void stop_test_motor1() {
	taskDelete(testMotor1PID);
	sysOutByte(0x181,M1_INHIB|M2_INHIB);
}

void stop_test_motor2() {
	taskDelete(testMotor2PID);
	sysOutByte(0x181,M1_INHIB|M2_INHIB);
}
