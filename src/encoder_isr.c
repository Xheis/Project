#include <avr/interrupt.h>
#include "lib/encoder.h"

ISR(INT0_vect)
{
    encoder_edge_A_isr();
}

ISR(INT1_vect)
{
    encoder_edge_B_isr();
}