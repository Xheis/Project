#ifndef _STEPPER_H
#define _STEPPER_H

#include <stdio.h>
#include <stdlib.h>

void stepper_init(void);
void test_stepper(void);			//Configure ADC and Timer 1
void set_dir(int,int);
void set_speed(int,int);
int get_pwm(float V);

typedef struct
{
	float K; // Motor constant [N.m/A] or [V.s/rad]
	float r; // Wheel radius [m]
	float eta; // Gearbox efficiency [-]
	float N; // Gear ratio [-]
	float taum; // Motor static friction torque [N.m]
	float R; // Motor armature resistance [Ohms]
} MOTOR_PARAM_T;

#endif
