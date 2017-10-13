#include <stdint.h>
#include <avr/io.h>
#include "startup.h"

int dummyData = 0;
int startup()
{
    dummyData;
}

void ChangeDummy(int newData)
{
    dummyData = newData;
}