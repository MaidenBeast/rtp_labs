#ifndef __motor1h
#define __motor1h

void motor1();
void rotateMotor1(int n_rotations);

motor_direction_t motor1_direction;
motor_speed_t motor1_speed;

int counter_motor1_steps;		//48 full-steps per rotation

#endif
