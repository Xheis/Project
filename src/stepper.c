#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "stepper.h"
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h> // atoi
#include <ctype.h>
#include <string.h>
#include <inttypes.h> // For PRIxx and SCNxx macros
#include <avr/pgmspace.h>
#include "cmd_line_buffer.h"

//					   L,R
static int EN_PIN[] = {4,7};//{2,5}; 
static int DIR_PIN[]= {2,5};//{3,6};
static int STEP_PIN[] = {3,6};//{4,7};

// Configure Timer1
void stepper_init(void)
{
	DDRD |= (1<<EN_PIN[0])|(1<<EN_PIN[1])|(1<<DIR_PIN[0])|(1<<DIR_PIN[1])|(1<<STEP_PIN[0])|(1<<STEP_PIN[1]);     // Configure PORTD2 -> PORTD7 as outputS
	PORTD &= ~(1<<EN_PIN[0]);     
	PORTD &= ~(1<<EN_PIN[1]);                 // Enable driver
}

void test_stepper()
{
	int x,y;
  
      DDRD |= (1<<EN_PIN[0])|(1<<EN_PIN[1])|(1<<DIR_PIN[0])|(1<<DIR_PIN[1])|(1<<STEP_PIN[0])|(1<<STEP_PIN[1]);     // Configure PORTD5, PORTD6, PORTD7 as output

	
    while (1) 
    {
		PORTD |= (1<<DIR_PIN[1]);
		PORTD &= ~(1<<DIR_PIN[1]);                 //Make PORTD6 high to rotate motor in clockwise direction

		for(x=0; x<100; x++)              //Give 50 pulses to rotate stepper motor by 90 degree's in full step mode
		{
		 for(y=0; y<10; y++)
		 {
		  PORTD |=(1<<STEP_PIN[0]);
		  PORTD |=(1<<STEP_PIN[1]);
		  _delay_us(700);
		  PORTD &=~(1<<STEP_PIN[0]);
		  PORTD &=~(1<<STEP_PIN[1]);
		   _delay_us(700);
		 }
		 printf_P(PSTR("step clockwis\n"));
		 _delay_ms(10);
		}
		
		PORTD &= ~(1<<DIR_PIN[0]);                //Make PORTD6 high to rotate motor in clockwise direction
		PORTD |= (1<<DIR_PIN[0]);             //Make PORTD6 high to rotate motor in anti-clockwise direction
		
		for(x=0; x<100; x++)             //Give 50 pulses to rotate stepper motor by 90 degree's in full step mode
		{
			for(y=0; y<10; y++)
			{
				PORTD |=(1<<STEP_PIN[0]);
				PORTD |=(1<<STEP_PIN[1]);
				_delay_us(700);
				PORTD &=~(1<<STEP_PIN[0]);
				PORTD &=~(1<<STEP_PIN[1]);
				_delay_us(700);
			}
		 printf_P(PSTR("step anti-clockwise\n"));
			_delay_ms(30);             
		}
	
	}
}
/*
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
			set_dir(1,1);
			speed = speed*-1;
		}else{
			set_dir(2);
		}
		OCR1AL = speed;

	}
	else if (stepper == RIGHT)
	{
		if(speed<0){
			set_dir(1);
			speed = speed*-1;
		}else{
			set_dir(2);
		}
		
		OCR1BL = speed;

	}
	else
	{
		//Error
		//testing commit
	}
}
*/
