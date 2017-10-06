F_CPU = 14745600

UNITY_ROOT = ../Unity

TARGET_BASE = project
TARGET_ELF = bin/$(TARGET_BASE).elf
TARGET_HEX = bin/$(TARGET_BASE).hex

TARGET_TEST_BASE = test_$(TARGET_BASE)
TARGET_TEST = test/bin/$(TARGET_TEST_BASE).exe

COMMONDEFS = -DF_CPU=$(F_CPU)UL

CDEFS = $(COMMONDEFS)

CFLAGS  = -std=c99
CFLAGS += $(CDEFS)
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition
CFLAGS += -Wpointer-to-int-cast
CFLAGS += -Wcomment
CFLAGS += -Wtype-limits

# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# PRINTF_LIB =
# PRINTF_LIB = $(PRINTF_LIB_MIN)
PRINTF_LIB = $(PRINTF_LIB_FLOAT)

# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# SCANF_LIB =
# SCANF_LIB = $(SCANF_LIB_MIN)
SCANF_LIB = $(SCANF_LIB_FLOAT)

MATH_LIB = -lm

# AVR Linker flags
LDFLAGS_AVR = $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)

SRC_COMMON = \
	src/circ_buffer.c \
	src/cmd_line_buffer.c \
	src/cmd_parser.c \
	src/controller.c \
	src/dimmer.c \
	src/encoder.c \
	src/encoder_isr.c \
	src/led.c \
	src/log_data.c \
	src/main.c \
	src/potentiometer.c \
	src/sin_table.c \
	src/task.c \
	src/task_isr.c \
	src/uart.c \
	src/uart_isr.c

SRC_TEST = \
	$(UNITY_ROOT)/src/unity.c \
	$(UNITY_ROOT)/extras/fixture/src/unity_fixture.c \
	$(SRC_COMMON) \
	mock/avr/mock_sfr.c \
	test/src/iospy.c \
	test/src/all_tests.c \
	test/src/test_led.c \
	test/src/test_encoder.c \
	test/src/test_potentiometer.c \
	test/src/test_circ_buffer.c \
	test/src/test_cmd_line_buffer.c \
	test/src/test_iospy.c \
	test/src/test_cmd_parse.c \
	test/src/test_cmd_process.c \
	test/src/test_cmd_led.c \
	test/src/test_cmd_enc.c \
	test/src/test_cmd_pot.c \
	test/src/test_cmd_xy.c \
	test/src/test_controller.c \
	test/src/test_cmd_controller.c \
	test/src/test_task.c \
	test/src/test_sin_table.c

SRC_AVR = \
	$(SRC_COMMON)

INC_COMMON = \
	-Isrc

INC_TEST = \
	-I$(UNITY_ROOT)/src \
	-I$(UNITY_ROOT)/extras/fixture/src \
	$(INC_COMMON) \
	-Imock

INC_AVR = \
	$(INC_COMMON)

SYMBOLS =

MISC_TEST = -DUNITY_FLOAT_PRECISION=0.0001f
MISC_TEST += -Wl,-wrap,cmd_parse

all: path clean default commit

path:
	echo on
	echo $(cd)
	cd "C:/Users/Administrator/Documents/MCHA3000/Coding/workspace/Project/src"
	echo $(cd)
commit:
	commit.bat

default:
	avr-gcc -g -Os -mmcu=atmega32 $(CFLAGS) $(INC_AVR) $(SRC_AVR) -o $(TARGET_ELF) $(LDFLAGS_AVR)
	avr-objcopy -j .text -j .data -O ihex $(TARGET_ELF) $(TARGET_HEX)

clean_avr:
	rm -f $(TARGET_ELF) $(TARGET_HEX)

clean_test:
	rm -f $(TARGET_TEST)

clean: clean_test clean_avr

program: clean default
	avrdude -p atmega32 -c usbasp -P com4 -b 115200 -u -U flash:w:$(TARGET_HEX)
