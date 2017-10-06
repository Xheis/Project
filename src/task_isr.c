#include <avr/interrupt.h>
#include "lib/task.h"

ISR(TIMER2_COMP_vect)
{
    task_trigger_isr();
}