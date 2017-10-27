#ifndef _STEPPER_H
#define _STEPPER_H

#include <stdio.h>
#include <stdlib.h>

void stepper_init(void);
void test_stepper(void);			//Configure ADC and Timer 1
void move_set_dist(int);
//void move_set_dist(int, int);
void move_set_time(int);
void set_velocity(float);
void set_dir(char*);
void move_set_steps(int steps);

#endif
