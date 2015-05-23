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

	//flush and delete message queues
	if (m1MsgQId!=0) msgQDelete(m1MsgQId);
	if (m2MsgQId!=0) msgQDelete(m2MsgQId);

	//delete the semaphores
	if (semMotors!=0) semDelete(semMotors);
	if (semCounterMotor1!=0) semDelete(semCounterMotor1);
	if (semCounterMotor2!=0) semDelete(semCounterMotor2);

	//delete the motors tasks
	if (motor1_PID!=0) taskDelete(motor1_PID);
	if (motor1_PID!=0) taskDelete(motor2_PID);

	//delete the lamp&fan tasks
	if (lamp_PID!=0) taskDelete(lamp_PID);
	if (fan_PID!=0) taskDelete(fan_PID);

	//turn off the motors
	sysOutByte(0x181, M2_INHIB|M1_INHIB);
	taskDelay(5);	//delay of 5ms for sendind the message

	//delete the main task
	if (lab3_PID!=0) taskDelete(lab3_PID);

}

void changeRunningMode(running_mode_t mode) {
	//input = -1;
	running_mode = mode;
}


/* 	The main task, that is responsible of:
		- managing running mode;
		- reading values from keyboard
		- parsing and sending keyboard input to motor message queues
*/
void lab3() {
	sysClkRateSet(1000);	//1000 ticks per second
	kernelTimeSlice(10); 	//enable round-robin scheduling process (10 ticks per time slice)

	input = 13; //D (entering into calibrarion mode)
	previousInput = -1;
	running_mode = CONFIG;			//"When the machine is started the tools can be placed in any position at all..."

	//configuration fan and lamp default mode
	changeFanMode(FAN_OFF);
	changeLampMode(LAMP_CONFIG);

	//lamp and fan turned off
	lamp_state = 0;
	fan_state = 0;

	//creating lamp and fan tasks
	lamp_PID = taskSpawn("lamp", 200, 0, 1000, lamp);
	fan_PID = taskSpawn("fan", 201, 0, 1000, fan);

	sysOutByte(0x184, 0xFF); 	//taking control of fan and lamp ports
	sysOutByte(0x181, M1_INHIB|M2_INHIB); //turn off motors

	while (1) {

		switch (running_mode) {
		case CONFIG:

			do { //reads from keyboard matrix
				previousInput = input;
				input = readKeyboard();
				//until the user digit something on the keyboard, and the button pressed is different than the previous one
			} while (input == -1 || previousInput==input);

			previousInput = input;

			switch (input) { //	3. The following 3 operator commands can be given:
			case 0:
				//End configuration mode and go to run-mode.
				changeRunningMode(RUN);
				changeFanMode(FAN_FIFTY_PERCENT); 		//because the machine are not running at the beginning
				changeLampMode(MACHINE_NOT_WORKING);

				//create the motor tasks
				motor1_PID = taskSpawn("motor1", 200, 0, 1000, motor1);
				motor2_PID = taskSpawn("motor2", 200, 0, 1000, motor2);

				//create semaphores
				semMotors = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
				semCounterMotor1 = semBCreate(SEM_Q_FIFO, SEM_FULL);
				semCounterMotor2 = semBCreate(SEM_Q_FIFO, SEM_FULL);

				//create message queues
				m1MsgQId = msgQCreate(10, 2, MSG_Q_FIFO);
				m2MsgQId = msgQCreate(10, 2, MSG_Q_FIFO);

				//turn on motors
				sysOutByte(0x181, 0x00);

				break;
			case 1:		//"Let motor 1 advance one half-step."
				sysOutByte(0x181,M2_INHIB|M1_STEP);
				taskDelay(25);
				sysOutByte(0x181,M2_INHIB);
				taskDelay(25);
				sysOutByte(0x181,M1_INHIB|M2_INHIB);	//"Turning off power immediately after."
				taskDelay(50);
				break;
			case 2:		//"Let motor 2 advance one half-step."
				sysOutByte(0x181,M1_INHIB|M2_STEP);
				taskDelay(25);
				sysOutByte(0x181,M1_INHIB);
				taskDelay(25);
				sysOutByte(0x181,M1_INHIB|M2_INHIB);	//"Turning off power immediately after."
				taskDelay(50);
				break;
			}
			break;
			case ERR:
				taskDelay(100);

				//input = -1;

				//"The motors should stop rotating immediately."
				msgQDelete(m1MsgQId);
				msgQDelete(m2MsgQId);

				//"The job queues for the engines are emptied."
				taskDelete(motor1_PID);
				taskDelete(motor2_PID);

				//"The warning lamp signals error mode by turning on for 1s and then off for 1s, repeatedly"
				changeLampMode(LAMP_ERR);

				// The system waits to be reset

				do { //reads from keyboard matrix
					previousInput = input;
					input = readKeyboard();
					//until the user digit something on the keyboard, and the button pressed is different than the previous one
				} while ((input == -1 && input != 13) || previousInput==input);

				previousInput = input;

				//D: Restart, the system enters calibration mode
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
				} while (input == -1 || (!(input>=0 && input<=6) && input!=10 && input!=11 && input!=15) || previousInput==input); //command to motors, or continue...

				previousInput = input;

				char msg[2];		//message buffer --> max 2 bytes, because the messages goes from "0" to "F" (plus the '\0' byte).

				switch (input) {
				case 0:
					//Stop both motors to the next safe-position
					semTake(semCounterMotor1, WAIT_FOREVER);	//take the counter semaphore (because maybe motor 1 is r/w-ing it)
					if (counter_motor1_steps > 0) {				//if motor 1 is working, of has already finished his stepping rotation
						//decrement the counter to the remaning steps for reaching the next safe-position
						int newCounter = counter_motor1_steps % QUARTER_ROTATION_STEPS;
						counter_motor1_steps = newCounter;
					}
					semGive(semCounterMotor1);

					semTake(semCounterMotor2, WAIT_FOREVER);	//take the counter semaphore (because maybe motor 2 is r/w-ing it)
					if (counter_motor2_steps > 0) {				//if motor 2 is working, of has already finished his stepping rotation.
						//decrement the counter to the remaning steps for reaching the next safe-position
						int newCounter = counter_motor2_steps % QUARTER_ROTATION_STEPS;
						counter_motor2_steps = newCounter;
					}
					semGive(semCounterMotor2);

					//sysOutByte(0x181, M2_INHIB|M1_INHIB);
					//taskDelay(5);

					semTake(semMotors, WAIT_FOREVER);
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
					sprintf(msg, "%1x", input);		//Parse integer input to string format that goes from "0" to "F"

					if ((input>= 1 && input<= 3) || input == 10) {	//the message is dedicated to motor1
						//Stop motor 2 at the next safe position
						semTake(semCounterMotor2, WAIT_FOREVER);	//take the counter semaphore (because maybe motor 2 is r/w-ing it)
						if (counter_motor2_steps > 0) {				//if motor 2 is working, of has already finished his stepping rotation.
							//decrement the counter to the remaning steps for reaching the next safe-position
							int newCounter = counter_motor2_steps % QUARTER_ROTATION_STEPS;
							counter_motor2_steps = newCounter;
						}
						semGive(semCounterMotor2);

						msgQSend(m1MsgQId, msg, 2, WAIT_FOREVER, MSG_PRI_NORMAL); //send the input to motor1 message queue
					}
					if ((input>= 4 && input<= 6) || input == 11) {	//the message is dedicated to motor2
						msgQSend(m2MsgQId, msg, 2, WAIT_FOREVER, MSG_PRI_NORMAL); //send the input to motor2 message queue
					}
					break;
				}
				break;
		}

	}
}

void start() {
	lab3_PID = taskSpawn("lab3", 201, 0, 1000, lab3); //create the main task (priority: 201)
}
