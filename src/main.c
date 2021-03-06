#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> 
#include <util/delay.h>
#include "encoder.h"
#include "uart.h"
#include "cmd_line_buffer.h"
#include "cmd_parser.h"
#include "task.h"
#include "startup.h"
#include "controller.h"
#include "log_data.h"

CLB_CREATE_STATIC(clb, 80);

int main(void)
{

    uart_init();
    // Send initial string
    printf_P(PSTR("\nWarming Up...\n"));
    _delay_ms(2000);
    printf_P(PSTR("\nC3206246 BUILD DATE: "__DATE__" "__TIME__"\n\n"));

    // Initialise modules
    encoder_init();
    task_init();
    //stepper_init();
    // Enable global interrupts
    sei();      

    // Wait a second at startup
    _delay_ms(2000);


    // Send initial string
    printf_P(PSTR("\nReady!\n\n"));

    for(;/*ever*/;)
    {
        if (task_triggered())
        {
            task_run();
        }
        else
        {
            clb_process(&clb);
        }
    }
    return 0;
}

