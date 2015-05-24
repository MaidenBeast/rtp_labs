#ifndef __commonh
#define __commonh

#define FAN_FLAG 	0x40
#define LAMP_FLAG 	0x80

#include "semLib.h"
#include "msgQLib.h"
#include "machine-utils.h"

typedef enum {
	CONFIG,		//configuration mode
	RUN,		//running mode
	ERR			//error mode
} running_mode_t;

running_mode_t running_mode;		//the running mode in an exact instant time


typedef enum {
	LAMP_OFF,						
	LAMP_CONFIG, 					//"This is done by PWM controlling the lamp going smoothly from 0% intensity to 100% intensity over 2s and then starting over abruptly from 0% again."
	MACHINE_WORKING,				//"Then the machines are working, the lamp should blink with 3 on/off flashes every second."
	MACHINE_NOT_WORKING,			//"When the machines are not, the lamp should smoothly go from zero intensity to full intensity, and then smoothly back to zero. Each such cycle should take 4s."
	LAMP_ERR						//"The warning lamp signals error mode by turning on for 1s and then off for 1s, repeatedly"
} lamp_mode_t;

typedef enum {
	FAN_OFF,
	FAN_ONE_HUNDRED_PERCENT, 		//"When the machines are operating, the fan should be working at 100%, ..."
	FAN_FIFTY_PERCENT				//"...otherwise the fan should be working at 50%"
} fan_mode_t;

/*		lamp and fan states		*/
int lamp_state;	//0 = off; 1 = on;
int fan_state; 	//0 = off; 1 = on;

/* 		motor state types		*/

typedef enum {
	CLOCKWISE,
	COUNTERCLOCKWISE
} motor_direction_t;

typedef enum {
	HIGH_SPEED,						//60 steps per second
	LOW_SPEED						//10 steps per second
} motor_speed_t;

#define FULL_ROTATION_STEPS 	48
#define HALF_ROTATION_STEPS 	24
#define QUARTER_ROTATION_STEPS 	12

/* 		message queues 		*/
MSG_Q_ID m1MsgQId;					//message queue used by motor1 task
MSG_Q_ID m2MsgQId;					//message queue used by motor2 task


/* 		  semaphores			*/
SEM_ID semMotors;					//binary semaphore about the usage of motors (0 if one motor is busy/working, 1 if all of them free/not working)
SEM_ID semCounterMotor1;			//semaphore dedicated to the counter of the first motor
SEM_ID semCounterMotor2;			//semaphore dedicated to the counter of the second motor

/*			tasks				*/
int motor1_PID;
int motor2_PID;

int lamp_PID;
int fan_PID;

int lab3_PID;

#endif
