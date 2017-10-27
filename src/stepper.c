#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/delay.h>
#include <util/delay.h>

#include <util/atomic.h>
//#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h> // atoi
#include <stdint.h>

#include <ctype.h>
#include <string.h>
#include <inttypes.h> // For PRIxx and SCNxx macros

#include "stepper.h"
#include "cmd_line_buffer.h"


//					   L,R
static int EN_PIN[] = {4,7};//{2,5}; 
static int DIR_PIN[]= {2,5};//{3,6};
static int STEP_PIN[] = {3,6};//{4,7};
const int STEP_DELAY = 400; //smoothing factor. Hand tweaked
const float WHEEL_RADIUS = 0.088; //88mm, polulu
const float RADIAN_PER_STEP = 0.0314159; //1.8degrees, IN THEORY. Tweak as needed. 
const int STEPS_PER_REV = 200; //Same as above
static int velocity_delay[] = {100,100};//{4,7};

// Configure Timer1
void stepper_init(void)
{
	DDRD |= (1<<EN_PIN[0])|(1<<EN_PIN[1])|(1<<DIR_PIN[0])|(1<<DIR_PIN[1])|(1<<STEP_PIN[0])|(1<<STEP_PIN[1]);     // Configure PORTD2 -> PORTD7 as outputS
	PORTD &= ~(1<<EN_PIN[0]);     
	PORTD &= ~(1<<EN_PIN[1]);                 // Enable driver

	//init as forwards
	set_dir("forwards");
}

void test_stepper()
{
	int x,y;
  
      DDRD |= (1<<EN_PIN[0])|(1<<EN_PIN[1])|(1<<DIR_PIN[0])|(1<<DIR_PIN[1])|(1<<STEP_PIN[0])|(1<<STEP_PIN[1]);     // Configure PORTD5, PORTD6, PORTD7 as output

	int step_Speed = 50;
	int step_distance = 25;
	int step_inbetween_delay = 400;
    while (1) 
    {
		PORTD |= (1<<DIR_PIN[0]);
		PORTD &= ~(1<<DIR_PIN[1]);                 //Make PORTD6 high to rotate motor in clockwise direction

		for(x=0; x<step_distance; x++)              //Give 50 pulses to rotate stepper motor by 90 degree's in full step mode
		{
		 for(y=0; y<10; y++)
		 {
		  PORTD |=(1<<STEP_PIN[0]);
		  PORTD |=(1<<STEP_PIN[1]);
		  _delay_us(step_inbetween_delay);
		  PORTD &=~(1<<STEP_PIN[0]);
		  PORTD &=~(1<<STEP_PIN[1]);
		   _delay_us(step_inbetween_delay);
		 }
		 //printf_P(PSTR("step clockwis\n"));
		 _delay_ms(step_Speed); //speed
		}
		
		PORTD &= ~(1<<DIR_PIN[0]);                //Make PORTD6 high to rotate motor in clockwise direction
		PORTD |= (1<<DIR_PIN[1]);             //Make PORTD6 high to rotate motor in anti-clockwise direction
		
		for(x=0; x<step_distance; x++)             //Give 50 pulses to rotate stepper motor by 90 degree's in full step mode
		{
			for(y=0; y<10; y++)
			{
				PORTD |=(1<<STEP_PIN[0]);
				PORTD |=(1<<STEP_PIN[1]);
				_delay_us(step_inbetween_delay);
				PORTD &=~(1<<STEP_PIN[0]);
				PORTD &=~(1<<STEP_PIN[1]);
				_delay_us(step_inbetween_delay);
			}
		 //printf_P(PSTR("step anti-clockwise\n"));
			_delay_ms(step_Speed);             
		}
	
	}
}

void set_dir(char* dir)
{

	if(!strcmp("forwards",dir))
	{
		PORTD |= (1<<DIR_PIN[0]);
		PORTD &= ~(1<<DIR_PIN[1]);                 //Make PORTD6 high to rotate motor in clockwise direction
	}
	else if(!strcmp("backwards",dir))
	{
		PORTD &= ~(1<<DIR_PIN[0]);                //Make PORTD6 high to rotate motor in clockwise direction
		PORTD |= (1<<DIR_PIN[1]);             //Make PORTD6 high to rotate motor in anti-clockwise direction
	}
	else if(!strcmp("left",dir))
	{
		PORTD &= ~(1<<DIR_PIN[0]);                //Make PORTD6 high to rotate motor in clockwise direction             //Make PORTD6 high to rotate motor in clockwise direction
		PORTD &= ~(1<<DIR_PIN[1]);
	}
	else if(!strcmp("right",dir))
	{
		PORTD |= (1<<DIR_PIN[0]);             //Make PORTD6 high to rotate motor in anti-clockwise direction
		PORTD |= (1<<DIR_PIN[1]);             //Make PORTD6 high to rotate motor in anti-clockwise direction
	}
	else
	{
		//ERROR
		printf_P(PSTR("ERROR\n"));
	}
	        printf_P(PSTR("Dir set to '%s' \n"), dir);
}
void set_velocity(int velocity)
{
	/* 
	 * https://www.embedded.com/design/mcus-processors-and-socs/4006438/Generate-stepper-motor-speed-profiles-in-real-time
	 *
	 * Time_Period can be calcualted by formula provided by embedded.com
	 * delta-Time = velocity_delay / Clock_Frequency
	 * Angular_Vel = (Angle_Per_Step*Clock_Frequency)/velocity_delay
	 *
	 * That is,
	 * Delta_Time*Clock_Frequency = Vel_Delay
	 * Angular_Vel = (Angle_Per_Step*Clock_Frequency)/(Delta-Time*Clock_Frequency)
	 * Angular_Vel = (Angle_Per_Step)/(Delta-Time)
	 * Angular_Vel = (1.8°)/Delta_Time
	 * Easy. We've proven that the steps we send per second, are proportional to RPM. Classic.
	 * 
	 * Time to look at that pesky delay, if we need a set angular_velocity
	 * Say we need, Omega RPM. 
	 * Omega = (1.8°)/Delta_Time
	 * Delta_Time = (0.0314159 Radians)/Omega
	 *
	 * So if we need, 0.05m/s (5cm/s), we sub in
	 * Knowing that..
	 * V = Omega*WHEEL_RADIUS
	 * Omega = V/WHEEL_RADIUS
	 * Delta_Time = (0.0314159)/(V/WHEEL_RADIUS)
	 * Delta_Time = (0.0314159*WHEEL_RADIUS)/(V)
	 * Delta_Time = (0.0314159*0.08)/(0.05) = 0.05026544 seconds 
	 * Delta_Time = (1000*0.0314159*0.08)/(0.05) = 50.26544 ms, which we can put into _delay_ms
	 *
	 */
	float Delta_Time = (1000*RADIAN_PER_STEP*0.08)/(velocity);
	//set both time delays to be the same. We might want to change this if we intended on having a full range of turning options
	velocity_delay[0]=(int)Delta_Time;
	velocity_delay[1]=(int)Delta_Time;
	printf_P(PSTR("Velocity '%d' set velocity_delay to '%d'\n"), velocity, velocity_delay[0]);
}

void move_set_time(int time_in_seconds)
{
	/* should be set by timer, or for now lets go time_in_seconds = velocity_delay
	 * velocity_delay is in ms
	 * velocity_delay[0] / 1000 = seconds 
	 * therefore, step_distance = time_in_seconds/(velocity_delay[0] / 1000)
	 */

	printf_P(PSTR("Time set for '%d' \n"), time_in_seconds);
	int step_distance_float = time_in_seconds/(velocity_delay[0] / 1000);
	int step_distance = (int)step_distance_float;
	int x,y;
	for(x=0; x<step_distance; x++)  //Give step_distance pulses to rotate stepper motor, to move cart Distance_mm
	{
		for(y=0; y<10; y++)
		{
		PORTD |=(1<<STEP_PIN[0]);
		PORTD |=(1<<STEP_PIN[1]);
		delay_us(STEP_DELAY);
		PORTD &=~(1<<STEP_PIN[0]);
		PORTD &=~(1<<STEP_PIN[1]);
		delay_us(STEP_DELAY);
		}
		//printf_P(PSTR("step clockwis\n"));
		delay_ms(velocity_delay[0]); //speed
	}
}

void move_set_dist(int Distance_mm)
{
	/*
	 * Distance = Revolutions_needed * WHEEL_RADIUS
	 * Distance_mm/WHEEL_RADIUS  = Revolutions_needed
	 * A Revolutions is 360/1.8 = 200 steps in theory.
	 * 
	 * Revolutions_needed = Distance_mm/WHEEL_RADIUS
	 * Steps_for_Distance_mm = (Distance_mm/WHEEL_RADIUS * STEPS_PER_REV * WHEEL_RADIUS)
	 */
	int step_distance = (Distance_mm/WHEEL_RADIUS * STEPS_PER_REV * WHEEL_RADIUS);
	int x,y;
	for(x=0; x<step_distance; x++)  //Give step_distance pulses to rotate stepper motor, to move cart Distance_mm
	{
	 for(y=0; y<10; y++)
	 {
	  PORTD |=(1<<STEP_PIN[0]);
	  PORTD |=(1<<STEP_PIN[1]);
	  delay_us(STEP_DELAY);
	  PORTD &=~(1<<STEP_PIN[0]);
	  PORTD &=~(1<<STEP_PIN[1]);
	   delay_us(STEP_DELAY);
	 }
	 //printf_P(PSTR("step clockwis\n"));
	 delay_ms(velocity_delay[0]); //speed
	}
}
// void move_set_dist(int Distance_mm,int stepper)
// {
// 	/*
// 	 * Distance = Revolutions_needed * WHEEL_RADIUS
// 	 * Distance_mm/WHEEL_RADIUS  = Revolutions_needed
// 	 * A Revolutions is 360/1.8 = 200 steps in theory.
// 	 * 
// 	 * Revolutions_needed = Distance_mm/WHEEL_RADIUS
// 	 * Steps_for_Distance_mm = (Distance_mm/WHEEL_RADIUS * STEPS_PER_REV * WHEEL_RADIUS)
// 	 */

// 	int step_distance = (Distance_mm/WHEEL_RADIUS * STEPS_PER_REV * WHEEL_RADIUS);
// 	int x,y;
// 	for(x=0; x<step_distance; x++)  //Give step_distance pulses to rotate stepper motor, to move cart Distance_mm
// 	{
// 	 for(y=0; y<10; y++)
// 	 {
// 	  PORTD |=(1<<STEP_PIN[stepper]);
// 	  delay_us(STEP_DELAY);
// 	  PORTD &=~(1<<STEP_PIN[stepper]);
// 	   delay_us(STEP_DELAY);
// 	 }
// 	 //printf_P(PSTR("step clockwis\n"));
// 	 _delay_ms(velocity_delay[stepper]); //speed
// 	}
// } 


/*
FORGIVE ME FOR I HAVE SIIIIINNNED 
*/
void delay_ms(uint16_t count)
{
  while(count--)
  {
    _delay_ms(1);
  }
}

void delay_us(uint16_t count)
{
  while(count--) 
  {
    _delay_us(1);
  }
}