#ifndef __commonh
#define __commonh

#include "semLib.h"
#include "msgQLib.h"
#include "machine-utils.h"

enum running_mode_t {
	CONFIG,		//configuration mode
	RUN,		//running mode
	ERROR		//error mode
};

extern enum running_mode_t running_mode;		//the running mode in an exact instant time


enum lamp_mode_t {
	OFF,
	CONFIG, 				//"This is done by PWM controlling the lamp going smoothly from 0% intensity to 100% intensity over 2s and then starting over abruptly from 0% again."
	WORKING,				//" hen the machines are working, the lamp should blink with 3 on/off flashes every second."
	NOT_WORKING,			//"When the machines are not, the lamp should smoothly go from zero intensity to full intensity, and then smoothly back to zero. Each such cycle should take 4s."
	ERROR					//"The warning lamp signals error mode by turning on for 1s and then off for 1s, repeatedly
};

extern enum lamp_mode lamp_mode;

enum fan_mode_t {
	OFF,
	ONE_HUNDRED_PERCENT, 	//"When the machines are operating, the fan should be working at 100%, ..."
	FIFTY_PERCENT			//"...otherwise the fan hould be working at 50%"
};

extern enum fan_mode_t fan_mode;


/* 		message queues 		*/
MSG_Q_ID runningMsgQId;		//message queue used by the running-mode tasks (motor1 and motor2)

/* 		semaphores			*/
SEM_ID semMotor1;			//binary semaphore about the usage of motor1 (0 if busy/working, 1 if free/not working)
SEM_ID semMotor2;			//binary semaphore about the usage of motor2 (0 if busy/working, 1 if free/not working)



#endif