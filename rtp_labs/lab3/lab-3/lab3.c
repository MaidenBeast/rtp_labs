#include "common.h"
#include "lamp.h"
#include "fan.h"
#include "motor1.h"
#include "motor2.h"

int input;
int previousInput;

void stop() {
	changeLampMode(LAMP_OFF);	//turn of the lamp
	changeFanMode(FAN_OFF);		//turn off the fan

	taskDelay(100);

	if (m1MsgQId!=0) msgQDelete(m1MsgQId);
	if (m2MsgQId!=0) msgQDelete(m2MsgQId);

	if (semMotors!=0) semDelete(semMotors);

	if (motor1_PID!=0) taskDelete(motor1_PID);
	if (motor1_PID!=0) taskDelete(motor2_PID);

	if (lamp_PID!=0) taskDelete(lamp_PID);
	if (fan_PID!=0) taskDelete(fan_PID);

	sysOutByte(0x181, M2_INHIB|M1_INHIB);
	taskDelay(5);

	if (lab3_PID!=0) taskDelete(lab3_PID);

}

void changeRunningMode(running_mode_t mode) {
	//input = -1;
	running_mode = mode;
}

void lab3() {
	sysClkRateSet(1000);
	kernelTimeSlice(10); //enable round-robin scheduling process

	input = 13; //D (entering into calibrarion mode)
	previousInput = -1;
	running_mode = CONFIG;
	changeFanMode(FAN_OFF);
	changeLampMode(LAMP_CONFIG);

	lamp_state = 0;
	fan_state = 0;

	lamp_PID = taskSpawn("lamp", 200, 0, 1000, lamp);
	fan_PID = taskSpawn("fan", 201, 0, 1000, fan);

	sysOutByte(0x184, 0xFF); //taking control of fan and lamp ports
	sysOutByte(0x181,M1_INHIB|M2_INHIB);

	while (1) {

		switch (running_mode) {
		case CONFIG:

			do { //reads from keyboard matrix until the user digit something on the keyboard
				previousInput = input;
				input = readKeyboard();
			} while (input == -1 || previousInput==input);

			previousInput = input;

			switch (input) { //	3. The following 3 operator commands can be given:
			case 0:
				//End configuration mode and go to run-mode.
				changeRunningMode(RUN);
				changeFanMode(FAN_FIFTY_PERCENT); //because the machine are not running at the beginning
				changeLampMode(MACHINE_NOT_WORKING);

				motor1_PID = taskSpawn("motor1", 200, 0, 1000, motor1);
				motor2_PID = taskSpawn("motor2", 201, 0, 1000, motor2);

				semMotors = semBCreate(SEM_Q_PRIORITY, SEM_FULL);

				m1MsgQId = msgQCreate(10, 2, MSG_Q_FIFO);
				m2MsgQId = msgQCreate(10, 2, MSG_Q_FIFO);

				sysOutByte(0x181, 0x00);

				break;
			case 1:
				sysOutByte(0x181,M2_INHIB|M1_STEP);
				taskDelay(25);
				sysOutByte(0x181,M2_INHIB);
				taskDelay(25);
				sysOutByte(0x181,M1_INHIB|M2_INHIB);
				taskDelay(50);
				break;
			case 2:
				sysOutByte(0x181,M1_INHIB|M2_STEP);
				taskDelay(25);
				sysOutByte(0x181,M1_INHIB);
				taskDelay(25);
				sysOutByte(0x181,M1_INHIB|M2_INHIB);
				taskDelay(50);
				break;
			}
			break;
			case ERR:
				taskDelay(100);

				//input = -1;

				//The job queues for the engines are emptied;
				msgQDelete(m1MsgQId);
				msgQDelete(m2MsgQId);

				taskDelete(motor1_PID);
				taskDelete(motor2_PID);

				//The warning lamp signals error mode by turning on for 1s and then off for 1s, repeatedly
				changeLampMode(LAMP_ERR);

				// The system waits to be reset

				do { //reads from keyboard matrix until the user digits something on the keyboard
					previousInput = input;
					input = readKeyboard();
					//taskDelay(10);
				} while ((input == -1 && input != 13) || previousInput==input); //D: Restart, the system enters calibration mode

				previousInput = input;

				changeRunningMode(CONFIG);
				changeFanMode(FAN_OFF);
				changeLampMode(LAMP_CONFIG);

				sysOutByte(0x181,M2_INHIB|M1_INHIB);

				break;
			default: 	//RUN
				taskDelay(100);

				//input = -1;

				do { //reads from keyboard matrix until the user digit something on the keyboard
					previousInput = input;
					input = readKeyboard();
					//taskDelay(10);
				} while (input == -1 || (!(input>=0 && input<=6) && input!=10 && input!=11 && input!=15) || previousInput==input);

				previousInput = input;

				char msg[2];

				switch (input) {
				case 0:
					//TODO: Stop the motors at the next safe position
					//sysOutByte(0x181, M2_INHIB|M1_INHIB);
					//taskDelay(5);
					stop();				//shutdown all processes
					break;
				case 15: //F: Emergency stop.
					sysOutByte(0x181,M2_INHIB|M1_INHIB);
					taskDelay(5);
					changeFanMode(FAN_OFF);		//turns off the fan
					changeLampMode(LAMP_OFF);	//turns off the lamp
					changeRunningMode(ERR); //and enter error mode
					break;
				default:
					sprintf(msg, "%1x", input);

					if ((input>= 1 && input<= 3) || input == 10) {
						msgQSend(m1MsgQId, msg, 2, WAIT_FOREVER, MSG_PRI_NORMAL); //send the input to motor1 message queue
					}
					if ((input>= 4 && input<= 6) || input == 11) {
						msgQSend(m2MsgQId, msg, 2, WAIT_FOREVER, MSG_PRI_NORMAL); //send the input to motor2 message queue
					}
					break;
				}
				break;
		}

	}
}

void start() {
	lab3_PID = taskSpawn("lab3", 201, 0, 1000, lab3);
}
