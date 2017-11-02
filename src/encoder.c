#include <stdint.h>
#include <avr/io.h>
#include "encoder.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // atoi

#include <ctype.h>
#include <string.h>
#include <inttypes.h> // For PRIxx and SCNxx macros
#include <avr/pgmspace.h>

static int32_t _count = 0;

// Forward decl of private helper functions
static void _encoder_enable_interrupts(void);
static void _encoder_disable_interrupts(void);

const A=2;
const B=3;

    //A = PD2 = int0
    //B = PD3 = int1

// const char ttable[6][4] = {
//   {0x3 , 0x2, 0x1,  0x0}, {0x83, 0x0, 0x1,  0x0},
//   {0x43, 0x2, 0x0,  0x0}, {0x3 , 0x5, 0x4,  0x0},
//   {0x3 , 0x3, 0x4, 0x40}, {0x3 , 0x5, 0x3, 0x80}
// };
// Use the full-step state table (emits a code at 00 only)
const char ttable[7][4] = {
  {0x0, 0x2, 0x4,  0x0}, {0x3, 0x0, 0x1, 0x40},
  {0x3, 0x2, 0x0,  0x0}, {0x3, 0x2, 0x1,  0x0},
  {0x6, 0x0, 0x4,  0x0}, {0x6, 0x5, 0x0, 0x80},
  {0x6, 0x5, 0x4,  0x0},
};
// // No complete step yet.
// #define DIR_NONE 0x0
// // Clockwise step.
// #define DIR_CW 0x10
// // Anti-clockwise step.
// #define DIR_CCW 0x20

// #define R_START 0x0

// #define R_CCW_BEGIN 0x1
// #define R_CW_BEGIN 0x2
// #define R_START_M 0x3
// #define R_CW_BEGIN_M 0x4
// #define R_CCW_BEGIN_M 0x5
// const unsigned char ttable[6][4] = {
//   // R_START (00)
//   {R_START_M,            R_CW_BEGIN,     R_CCW_BEGIN,  R_START},
//   // R_CCW_BEGIN
//   {R_START_M | DIR_CCW, R_START,        R_CCW_BEGIN,  R_START},
//   // R_CW_BEGIN
//   {R_START_M | DIR_CW,  R_CW_BEGIN,     R_START,      R_START},
//   // R_START_M (11)
//   {R_START_M,            R_CCW_BEGIN_M,  R_CW_BEGIN_M, R_START},
//   // R_CW_BEGIN_M
//   {R_START_M,            R_START_M,      R_CW_BEGIN_M, R_START | DIR_CW},
//   // R_CCW_BEGIN_M
//   {R_START_M,            R_CCW_BEGIN_M,  R_START_M,    R_START | DIR_CCW},
// };

/* Read input pins and process for events. Call this either from a
 * loop or an interrupt (eg pin change or timer).
 *
 * Returns 0 on no event, otherwise 0x80 or 0x40 depending on the direction.
 */
char rotary_process(uint8_t A_in, uint8_t B_in);
volatile char state = 0;
char rotary_process(uint8_t A_in, uint8_t B_in)
{
    char pinstate = (A_in << 1) | B_in; //Create a hex 0xAB, to cross reference with our state LUT
    state = ttable[state & 0xf][pinstate];
    return (state & 0xc0);//0xc0);
}

void checkEnc(uint8_t A_in, uint8_t B_in);
void checkEnc(uint8_t A_in, uint8_t B_in) 
{
    char result = rotary_process(A_in,B_in);
    if (result)
    {
        if (result == 0x40)
        {
            printf_P(PSTR("Clockwise\n"));
            _count--;   
        }
        else
        {
            printf_P(PSTR("Anti-clockwise\n"));
            _count++;   
        }
    }
    else
    {
    }
}


void encoder_init(void)
{
    // TODO: Enable pullup resistors (if not done in hardware)
    //done in hardware [tick]
    // TODO: Configure interrupt edge triggers for INT0 and INT1
    //to do
    MCUCR = (1<<ISC11) | (0<<ISC10) |   //int1 ISC11:0 ISC10: 1 = Any logical change, from table 16-1 from datasheet
            (1<<ISC01) | (0<<ISC00);    //int0 ISC01:0 ISC00: 1 = Any logical change, from table 16-1 from datasheet

    DDRD |= (0<<A) | (0<<B);
    PORTD |= (1<<A) | (1<<B);
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

    int A = !(PIND & _BV(PD2));
    int B = !(PIND & _BV(PD3));
    if (A==B) _count++;
    else _count--;
}

void encoder_edge_B_isr(void)
{
    // TODO: Implement A edge logic to increment or decrement _count
    //A = PD2 = int0
    //B = PD3 = int1

    int A = !(PIND & _BV(PD2));
    int B = !(PIND & _BV(PD3));
    if (A==B) _count--;
    else _count++;
}


// void encoder_edge_A_isr(void)
// {
//     // TODO: Implement A edge logic to increment or decrement _count
//     //A = PD2 = int0
//     //B = PD3 = int1

//     int A = !(PIND & _BV(PD2));
//     int B = !(PIND & _BV(PD3));
//     if (A==B) _count++;
//     else _count--;
// }

// void encoder_edge_B_isr(void)
// {
//     // TODO: Implement A edge logic to increment or decrement _count
//     //A = PD2 = int0
//     //B = PD3 = int1

//     int A = !(PIND & _BV(PD2));
//     int B = !(PIND & _BV(PD3));
//     if (A==B) _count--;
//     else _count++;
// }

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
    GICR |= (1<<INT0)| (1<<INT1);
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

  