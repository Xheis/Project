#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <math.h>
#include <stdbool.h>
#include <inttypes.h>
#include "task.h"

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
    // TODO: Setup Timer2 to generate compare match interrupts at 100Hz

    task_disable();
    _task_callback = NULL;
}

void task_enable(void)
{
    _task_trigger_count = 0;

    TCNT2 = 0;                      // reset counter
    _task_enable_trigger_isr();     // enable output compare interrupt
    /*TCCR2 |= ???;*/               // TODO: start timer (connect clock source)
}

void task_disable(void)
{
    _task_disable_trigger_isr();    // disable output compare interrupt
    /*TCCR2 &= ???;*/               // TODO: stop timer (disconnect clock source)

    _task_trigger_count = 0;
}

uint8_t _task_get_ticks_per_trigger(void)
{
    return OCR2;
}

void _task_enable_trigger_isr(void)
{
    /*TIMSK |= ???;*/       // TODO: enable output compare interrupt
}

void _task_disable_trigger_isr(void)
{
    /*TIMSK &= ???;*/       // TODO: disable output compare interrupt
}

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
        printf_P(PSTR("*** Task called with NULL callback! ***\n"));
    }
}

void task_trigger_isr(void)
{
    ++_task_trigger_count;
}
