#ifndef __motor2h
#define __motor2h

void motor2();
void rotateMotor2(int n_rotations);

motor_direction_t motor2_direction;
motor_speed_t motor2_speed;

int counter_motor2_steps;		//48 full-steps per rotation

#endif
