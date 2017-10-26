#include "unity_fixture.h"
#include <stdint.h>
#include "avr/io.h"
#include "avr/mock_sfr.h"
#include "Startup.h"

TEST_GROUP(Startup);

TEST_SETUP(Startup)
{
    //encoder_init();
}

TEST_TEAR_DOWN(Startup)
{
}

/* Tests if our dummy in main.c data = 0 */
TEST(Startup, Test1)
{
	ChangeDummy(0);
    TEST_ASSERT_EQUAL_INT32(0, Startup());
}

TEST(Startup, Test2)
{
	ChangeDummy(10);
    TEST_ASSERT_EQUAL_INT32(10, Startup());
}

/*TEST_GROUP_RUNNER(Startup)
{
    RUN_TEST_CASE(Startup, Test1);
    RUN_TEST_CASE(Startup, Test2);
}*/