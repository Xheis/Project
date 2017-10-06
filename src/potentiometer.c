#include <stdint.h>
#include <avr/io.h>
#include "lib/potentiometer.h"

void pot_init(void)
{
    // TODO: Configure ADC
ADCSRA = (1<<ADEN) | (6<<ADPS0);
ADMUX=(1<<REFS0);                         // For Aref=AVcc;
//ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Rrescalar div factor =128
}

uint16_t pot_get_value(void)
{
	
    // TODO: Do a single ADC conversion and return 10-bit result
    ADCSRA |= (1<<ADSC); // single ADC conversion is started by moving a 1 into adsc register 
	
	while ( (ADCSRA&(0<<ADIF) && (ADCSRA) ))
	{
	//WAIT UNTIL BIT IS SET	
	}	
	//return 10bit result
	return (uint16_t)ADCW;
}