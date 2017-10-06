#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "stepper.h"

CONST LEFT=1
CONST RIGHT=2
// Configure Timer1
void stepper_init(void)
{

}

void set_dir(int stepper, int dir)
{
	if (stepper == LEFT)
	{
		if(dir == 1)
		{
			PORTC |= (1<<PORTC3);
			PORTC &= ~(1<<PORTC4);
		}else if(dir == 2)
		{
			PORTC |= (1<<PORTC4);
			PORTC &= ~(1<<PORTC3);
		}else if(dir == 0)
		{
			PORTC &= ~(1<<PORTC3);
			PORTC &= ~(1<<PORTC4);
		}

	}
	else if (stepper == RIGHT)
	{
		if(dir == 1)
		{
			PORTC |= (1<<PORTC6);
			PORTC &= ~(1<<PORTC5);
		}else if(dir == 2)
		{
			PORTC |= (1<<PORTC5);
			PORTC &= ~(1<<PORTC6);
		}else if(dir == 0)
		{
			PORTC &= ~(1<<PORTC5);
			PORTC &= ~(1<<PORTC6);
		}

	}
	else
	{
		//Error
	}
}

void set_speed(int stepper, int speed)
{
	if (stepper == LEFT)
	{
		if(speed<0){
			set_dir1(1);
			speed = speed*-1;
		}else{
			set_dir1(2);
		}
		OCR1AL = speed;

	}
	else if (stepper == RIGHT)
	{
		if(speed<0){
			set_dir2(1);
			speed = speed*-1;
		}else{
			set_dir2(2);
		}
		
		OCR1BL = speed;

	}
	else
	{
		//Error
	}
}
