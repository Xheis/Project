#include <stdint.h>
#include <avr/io.h>
#include "Startup.h"

int dummyData = 0;
int Startup()
{
    return dummyData;
}

void ChangeDummy(int newData)
{
    dummyData = newData;
}