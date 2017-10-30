#include <stdint.h>
#include <avr/io.h>
#include "encoder.h"

static int32_t _count = 0;

// Forward decl of private helper functions
static void _encoder_enable_interrupts(void);
static void _encoder_disable_interrupts(void);

void encoder_init(void)
{
    // TODO: Enable pullup resistors (if not done in hardware)
    //done in hardware [tick]
    // TODO: Configure interrupt edge triggers for INT0 and INT1
    //to do
    MCUCR = (1<<ISC11) | (0<<ISC10) |   //int1 ISC11:0 ISC10: 1 = Any logical change, from table 16-1 from datasheet
            (1<<ISC01) | (0<<ISC00);    //int0 ISC01:0 ISC00: 1 = Any logical change, from table 16-1 from datasheet


    // Set default count
    _count = 0;

    // Enable interrupts INT0 and INT1
    _encoder_enable_interrupts();
}

void encoder_edge_A_isr(void)
{
    // TODO: Implement A edge logic to increment or decrement _count
    //A = PD2 = int0
    //B = PD3 = int1

    int A = !(PINA & _BV(PA0));
    int B = !(PINA & _BV(PA1));
    if (A==B) _count++;
    else _count--;
}

void encoder_edge_B_isr(void)
{
    // TODO: Implement A edge logic to increment or decrement _count
    //A = PD2 = int0
    //B = PD3 = int1

    int A = !(PINA & _BV(PD1));
    int B = !(PINA & _BV(PD0));
    if (A==B) _count--;
    else _count++;
}

void encoder_set_count(int32_t count)
{
    // Atomically set _count
    _encoder_disable_interrupts();
    _count = count;
    _encoder_enable_interrupts();
}

int32_t encoder_get_count(void)
{
    // Atomically read _count
    _encoder_disable_interrupts();
    uint32_t count = _count;
    _encoder_enable_interrupts();
    return count;
}

int32_t encoder_pop_count(void)
{
    // Atomically read and reset _count
    _encoder_disable_interrupts();
    int32_t count = _count;
    _count = 0;
    _encoder_enable_interrupts();
    return count;
}

void _encoder_enable_interrupts(void)
{
    // TODO: Enable INT0 and INT1 interrupts
    //Enable INT0
    GICR |= (1<<INT0) | (1<<INT1);
    //Enable INT1
   // GICR |= (1<<INT1);
}

void _encoder_disable_interrupts(void)
{
    // TODO: Disable INT0 and INT1 interrupts
    //Enable INT0
    GICR &= ~((1<<INT0) | (1<<INT1));
    //Enable INT1
   // GICR |= (0<<INT1);
}

