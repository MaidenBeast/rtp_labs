#include "common.h"
#include "lamp.h"
#include "fan.h"
#include "motor1.h"
#include "motor2.h"

int input;

void stop() {
	if (m1MsgQId!=0) msgQDelete(m1MsgQId);
	if (m2MsgQId!=0) msgQDelete(m2MsgQId);

	if (semMotor1!=0) semDelete(semMotor1);
	if (semMotor2!=0) semDelete(semMotor2);

	if (motor1_PID!=0) taskDelete(motor1_PID);
	if (motor1_PID!=0) taskDelete(motor2_PID);
	
	if (lamp_PID!=0) taskDelete(lamp_PID);
	if (fan_PID!=0) taskDelete(fan_PID);
	
	if (lab3_PID!=0) taskDelete(lab3_PID);
}

void changeRunningMode(running_mode_t mode) {
	input = -1;
	running_mode = mode;
}

void lab3() {
	sysClkRateSet(1000);
	kernelTimeSlice(10); //enable round-robin scheduling process
	
	input = 13; //D (entering into calibrarion mode)
	running_mode = CONFIG;
	changeFanMode(FAN_OFF);
	changeLampMode(LAMP_CONFIG);
	
	lamp_state = 0;
	fan_state = 0;

	lamp_PID = taskSpawn("lamp", 200, 0, 1000, lamp);
	fan_PID = taskSpawn("fan", 201, 0, 1000, fan);
	
	sysOutByte(0x184, 0xFF); //taking control of fan and lamp ports
	
	while (1) {
	
		switch (running_mode) {
			case CONFIG:

				do { //reads from keyboard matrix until the user digit something on the keyboard
					input = readKeyboard();
					//taskDelay(10);
				} while (input == -1);

				switch (input) { //	3. The following 3 operator commands can be given:
					case 0:
						//End configuration mode and go to run-mode.
						changeRunningMode(RUN);
						changeFanMode(FAN_FIFTY_PERCENT); //because the machine are not running at the beginning
						changeLampMode(MACHINE_NOT_WORKING);

						motor1_PID = taskSpawn("motor1", 210, 0, 1000, motor1);
						motor2_PID = taskSpawn("motor2", 211, 0, 1000, motor2);

						semMotor1 = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
						semMotor2 = semBCreate(SEM_Q_PRIORITY, SEM_FULL);

						m1MsgQId = msgQCreate(10, sizeof(char), MSG_Q_FIFO);
						m2MsgQId = msgQCreate(10, sizeof(char), MSG_Q_FIFO);
						
						break;
					case 1:
						//TODO: Let motor 1 advance one half-step. Turning off power immediately after.
						break;
					case 2:
						//TODO: Let motor 2 advance one half-step. Turning off power immediately after.
						break;
				}
				break;
			case ERR:
				taskDelay(100);
				//TODO: The motors should stop rotating immediately

				//The job queues for the engines are emptied;
				msgQDelete(m1MsgQId);
				msgQDelete(m2MsgQId);

				//The warning lamp signals error mode by turning on for 1s and then off for 1s, repeatedly
				changeLampMode(LAMP_ERR);

				// The system waits to be reset

				do { //reads from keyboard matrix until the user digits something on the keyboard
					input = readKeyboard();
					//taskDelay(10);
				} while (input == -1 && input != 13); //D: Restart, the system enters calibration mode

				taskDelete(motor1_PID);
				taskDelete(motor2_PID);

				changeRunningMode(CONFIG);
				changeFanMode(FAN_OFF);
				changeLampMode(LAMP_CONFIG);

				break;
			default: 	//RUN
				taskDelay(100);
				
				do { //reads from keyboard matrix until the user digit something on the keyboard
					input = readKeyboard();
					//taskDelay(10);
				} while (input == -1 || (!(input>=0 && input<=6) && input!=10 && input!=11 && input!=15));

				switch (input) {
					case 0:
						//TODO: Stop the motors at the next safe position
						//TODO: Release the power to the engines		
						changeLampMode(LAMP_OFF);	//turn of the lamp
						changeFanMode(FAN_OFF);		//turn off the fan
						stop();				//shutdown all processes
						break;
					case 15: //F: Emergency stop.
						//TODO: Immediately stop all the stepping of the engines
						changeFanMode(FAN_OFF);		//turns off the fan
						changeLampMode(LAMP_OFF);	//turns off the lamp
						changeRunningMode(ERR); //and enter error mode
						break;
					default:
						if ((input>= 1 && input<= 3) || input == 10) {
							msgQSend(m1MsgQId, (char)input, 1, WAIT_FOREVER, MSG_PRI_NORMAL); //send the input to motor1 message queue
						}
						if ((input>= 4 && input<= 6) || input == 11) {
							msgQSend(m2MsgQId, (char)input, 1, WAIT_FOREVER, MSG_PRI_NORMAL); //send the input to motor2 message queue
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
