#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <math.h>
#include <stdbool.h>
#include <inttypes.h>
#include "task.h"

#include <avr/io.h>

static TASK_CALLBACK_T _task_callback;         // Task callback function pointer
static volatile uint8_t _task_trigger_count;

static uint8_t  _task_get_ticks_per_trigger(void);
static void     _task_enable_trigger_isr(void);
static void     _task_disable_trigger_isr(void);
static bool     _task_is_trigger_isr_enabled(void);

uint8_t task_triggered(void)
{
    return _task_trigger_count;
}

void task_init(void)
{
    printf_P(PSTR("Start task_int\n"));
        
    // TODO: Setup Timer2 to generate compare match interrupts at 100Hz

    /*TCCR0 |= (1<<WGM00)|(1<<COM01)|(1<<WGM01)|(1<<CS00); // initialize timer0 in PWM mode
    //DDRB |= (1<<PB3); // make OC0 pin as output pin
    DDRB |= _BV(PB3);
    OCR0 = 0; */

    DDRD |= _BV(PD4);
    // Set timer to CTC mode and prescaler to 1
    TCCR2 = (1<<WGM21)|(0<<WGM20) | //from table 38 in doc2503, setting up [1,0] CTC
            (1<<COM21)|(1<<COM20) |
            _BV(FOC2);// | //from table 39 in dcos2503, setting [1,1] sets OCR1 on COMPARE-MATCH
            //(1<<CS22)|(0<<CS21)|(1<<CS20);  //from table 42 in doc2503, 1024 Prescalling
    /*
     * sets a clk/1. Might need change?  Formula from page 75
        100Hz = 14.7456 MHz/(2 * N (1 + OCRn))
        (2 * N (1 + OCRn)) = 14.7466*10^3/100
        (1 + OCRn) = 14.7466*10^6/(100*2*N)
        OCRn = 14.7466*10^6/(100*2*N)-1
        OCRn = 14.7466*10^6/(100*2*1024)-1
        OCRn = 71.0048
     */
    // Set TOP value for 100Hz operation (14.7456MHz XTAL)
    OCR2 = 0xFF;//71; 
    /*OCR0 is our Compare Match, e.g. our timer will cycle 0x00 -> 0xFF, 
    and when it hits 0xOCR0, it will invert it's output at OC0 */
    // Interrupt on compare
    //TIMSK |= 1<<OCIE0; 



    //DDRB |= _BV(PB3);
    //OCR2 = 0;


    task_disable();
    _task_callback = NULL;

    printf_P(PSTR("End task_int\n"));
}

void task_enable(void)
{

    printf_P(PSTR("Start task_enable\n"));
    _task_trigger_count = 0;
    printf_P(PSTR("1\n"));
    TCNT2 = 0;                      // reset counter
    
    /*TCCR2 |= ???;*/               // TODO: start timer (connect clock source)
    TCCR2 |= _BV(CS22)|_BV(CS20);//((1<<CS22)|(0<<CS21)|(1<<CS20));  //from table 42 in doc2503, 1024 Prescalling
    
    printf_P(PSTR("2\n"));
    _task_enable_trigger_isr();     // enable output compare interrupt
    
    printf_P(PSTR("3\n"));
    printf_P(PSTR("End task_enable\n"));
}

void task_disable(void)
{
    printf_P(PSTR("Start task_disable\n"));
    _task_disable_trigger_isr();    // disable output compare interrupt
    /*TCCR2 &= ???;*/               // TODO: stop timer (disconnect clock source)
    TCCR2 &= ~(_BV(CS22)|_BV(CS20));  //from table 42 in doc2503, disconnect

    _task_trigger_count = 0;
    printf_P(PSTR("End task_disable\n"));
}

uint8_t _task_get_ticks_per_trigger(void)
{
    return OCR2;
}

void _task_enable_trigger_isr(void)
{
    printf_P(PSTR("Start ISR\n"));
    TIMSK |= _BV(OCIE2);       // Enable output compare interrupt
    printf_P(PSTR("End ISR\n"));
}

void _task_disable_trigger_isr(void)
{
    printf_P(PSTR("Start ~ISR\n"));
    TIMSK &= ~_BV(OCIE2);       // Disable output compare interrupt
    printf_P(PSTR("End ~ISR\n"));
}

// void _task_enable_trigger_isr(void)
// {
//     /*TIMSK |= ???;*/       // TODO: enable output compare interrupt
//     TIMSK |= _BV(OCIE2);//(1<<OCIE2);
// }

// void _task_disable_trigger_isr(void)
// {
//     /*TIMSK &= ???;*/       // TODO: disable output compare interrupt
//     TIMSK &= ~(_BV(OCIE2));
// }

bool _task_is_trigger_isr_enabled(void)
{
    return bit_is_set(TIMSK, OCIE2);
}

bool task_is_enabled(void)
{
    return _task_is_trigger_isr_enabled();
}

void task_set(TASK_CALLBACK_T func)
{
    _task_callback = func;
}

TASK_CALLBACK_T task_get(void)
{
    return _task_callback;
}

uint16_t task_get_ticks(void)
{
    uint8_t ticks_per_trigger = _task_get_ticks_per_trigger();
    uint8_t trigger_count, ticks_since_last_trigger;

    // Synchronise read of trigger count and timer count if trigger isr enabled
    bool trigger_was_enabled    = _task_is_trigger_isr_enabled();
    if (trigger_was_enabled) _task_disable_trigger_isr();
    trigger_count               = _task_trigger_count;
    ticks_since_last_trigger    = TCNT2;
    if (trigger_was_enabled) _task_enable_trigger_isr();

    return (ticks_per_trigger*trigger_count + ticks_since_last_trigger);
}

void task_run(void)
{
    printf_P(PSTR("Start task_run\n"));
    if (_task_callback)
    {
        _task_trigger_count = 0;
        uint16_t tardiness = task_get_ticks(); // It's a real word (http://www.dictionary.com/browse/tardiness)
        _task_callback();
        uint16_t total = task_get_ticks();
        
        if (_task_trigger_count) // Check if the trigger occurred during the task
        {
            // If we get here, the task took longer than the timer period
            task_disable();
            uint16_t duration = total - tardiness;
            uint8_t ticks_per_trigger = _task_get_ticks_per_trigger();
            printf_P(PSTR("*** Task incomplete before next trigger! [tar/dur/max: %" PRIu16 "/%" PRIu16 "/%" PRIu8 "] ***\n"), tardiness, duration, ticks_per_trigger);
        }
    }
    else
    {
        task_disable();
        printf_P(PSTR("*** Task called with NULL callback! ***\n\n"));
    }
    printf_P(PSTR("End task_run\n"));
}

void task_trigger_isr(void)
{
    ++_task_trigger_count;
    PORTD ^= _BV(PD4);
}
