#include <stdint.h>
#include <avr/io.h>
#include "Startup.h"

int8_t dummyData = 0;
int8_t Startup()
{
    return dummyData;
}

void ChangeDummy(int8_t newData)
{
    dummyData = newData;
}