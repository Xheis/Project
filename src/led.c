#include <stdint.h>
#include <avr/io.h>
#include "led.h"


void led_init(void)
{
    TCCR0 |= (1<<WGM00)|(1<<COM01)|(1<<WGM01)|(1<<CS00); // initialize timer0 in PWM mode
	//DDRB |= (1<<PB3); // make OC0 pin as output pin
	DDRB |= _BV(PB3);
	OCR0 = 0;
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