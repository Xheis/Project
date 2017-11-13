#include "unity_fixture.h"
#include "avr/io.h"
#include "avr/mock_sfr.h"
#include "stepper.h"

TEST_GROUP(Stepper);

TEST_SETUP(Stepper)
{
    stepper_init();
}

TEST_TEAR_DOWN(Stepper)
{
}

TEST(Stepper, setDir) //Tests if our Direction method is working
{
	mock_portd = 0x00;
	set_dir('Backwards')
	TEST_ASSERT_BITS_HIGH_MESSAGE(0x20, mock_portd, "Expected 00100000");
	set_dir('Forwards')
	TEST_ASSERT_BITS_HIGH_MESSAGE(0x04, mock_portd, "Expected 00000100");
	set_dir('Left')
	TEST_ASSERT_BITS_HIGH_MESSAGE(0x00, mock_portd, "Expected 00000000");
	set_dir('Right')
	TEST_ASSERT_BITS_HIGH_MESSAGE(0x24, mock_portd, "Expected 00100100");
}

TEST_GROUP_RUNNER(Startup)
{
    RUN_TEST_CASE(Startup, setDir);
}