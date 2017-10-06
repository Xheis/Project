#include <stdint.h>
#include <avr/io.h>
#include "led.h"


void led_init(void)
{
    /*TCCR0 |= (1<<WGM00)|(1<<COM01)|(1<<WGM01)|(1<<CS00); // initialize timer0 in PWM mode
	//DDRB |= (1<<PB3); // make OC0 pin as output pin
	DDRB |= _BV(PB3);
	OCR0 = 0; */

	// Set timer to CTC mode and prescaler to 1/1024
	TCCR0 = (1<<WGM01)|(1<<WGM00) | //from table 38 in doc2503, setting up [1,1] Fast PWM
			(1<<COM01)|(1<<COM00) | //from table 40 in dcos2503, setting [1,1] Inverting Mode
			(1<<CS02)|(0<<CS01)|(0<<CS00);  //from table 42 in doc2503, 
	/*
	 * sets a clk/256. Might need change?  Formula from page 75 is f_OC0-PWM = (f_clk)/(N*256)
	 	14745600
	 */

	// Set TOP value for 49.45Hz operation (8MHz XTAL)
	OCR0 = 255; //OCR0 is our Compare Match, e.g. our timer will cycle 0x00 -> 0xFF, and when it hits 0xOCR0, it will invert it's output at OC0
	// Interrupt on compare
	//TIMSK |= 1<<OCIE0; 

}

void led_on(void)
{
    //PORTB |= _BV(PB3);
	OCR0 = 255;
}

void led_off(void)
{
   // PORTB &= ~(_BV(PB3));
	OCR0 = 0;
}

void led_set_brightness(uint8_t value)
{
    /*if (value)  led_on();
    else        led_off();*/
	OCR0 = value;

}

uint8_t led_get_brightness(void)
{
    /*if (bit_is_set(PINB, PB3))  return OCR0;
    else                        return 0;*/
	return OCR0;
}