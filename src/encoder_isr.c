#include <avr/interrupt.h>
#include "encoder.h"

static uint8_t A = 0;
static uint8_t B = 0;
ISR(INT0_vect)
{
    encoder_edge_A_isr();

	// A = !(PIND & _BV(PD2));
	// B = !(PIND & _BV(PD3));
 //    checkEnc(A,B);
}

ISR(INT1_vect)
{
    encoder_edge_B_isr();
	// A = !(PIND & _BV(PD2));
	// B = !(PIND & _BV(PD3));
 //    checkEnc(A,B);
}