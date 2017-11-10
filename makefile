# We try to detect the OS we are running on, and adjust commands as needed
ifeq ($(OS),Windows_NT)
	TARGET_TEST_EXTENSION=.exe
else
	TARGET_TEST_EXTENSION=.out
endif

F_CPU = 14745600

UNITY_ROOT = ../Unity

PATH_BASE = C:/Users/Administrator/Documents/MCHA3000/Coding/workspace/Project
TARGET_BASE = project
TARGET_ELF = $(PATH_BASE)/bin/$(TARGET_BASE).elf
TARGET_HEX = $(PATH_BASE)/bin/$(TARGET_BASE).hex

TARGET_TEST_BASE = test_$(TARGET_BASE)
TARGET_TEST = test/bin/$(TARGET_TEST_BASE)$(TARGET_TEST_EXTENSION)

COMMONDEFS = -DF_CPU=$(F_CPU)UL

CDEFS = $(COMMONDEFS)

CFLAGS  = -std=c99
CFLAGS += $(CDEFS)
# 			enables warnings
#CFLAGS += -Wall
# 			disables warnings
CFLAGS += -w
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
PRINTF_LIB = $(PRINTF_LIB_MIN)
PRINTF_LIB = $(PRINTF_LIB_FLOAT)

# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# SCANF_LIB =
SCANF_LIB = $(SCANF_LIB_MIN)
SCANF_LIB = $(SCANF_LIB_FLOAT)

MATH_LIB = -lm

# AVR Linker flags
LDFLAGS_AVR = $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)

SRC_COMMON = \
	src/encoder.c \
	src/circ_buffer.c \
	src/cmd_line_buffer.c \
	src/cmd_parser.c \
	src/stepper.c \
	src/Startup.c \
	src/controller.c \
	src/task.c \
	src/sin_table.c \
	src/log_data.c


SRC_TEST = \
	$(UNITY_ROOT)/src/unity.c \
	$(UNITY_ROOT)/extras/fixture/src/unity_fixture.c \
	$(SRC_COMMON) \
	mock/avr/mock_sfr.c \
	test/src/iospy.c \
	test/src/all_tests.c \
	test/src/test_encoder.c \
	test/src/test_circ_buffer.c \
	test/src/test_cmd_line_buffer.c \
	test/src/test_iospy.c \
	test/src/stdio_redirect.c \
	test/src/test_stdio_redirect.c \
	test/src/test_cmd_parse.c \
	test/src/test_cmd_process.c \
	test/src/test_cmd_enc.c \
	test/src/test_cmd_xy.c \
	test/src/test_controller.c \
	test/src/test_cmd_controller.c \
	test/src/test_task.c \
	test/src/test_sin_table.c \
	test/src/test_Startup.c

SRC_AVR = \
	$(SRC_COMMON) \
	src/main.c \
	src/encoder_isr.c \
	src/uart_isr.c \
	src/uart.c

INC_COMMON = \
	-Isrc

INC_TEST = \
	-I$(UNITY_ROOT)/src \
	-I$(UNITY_ROOT)/extras/fixture/src \
	$(INC_COMMON) \
	-Imock

INC_AVR = \
	$(INC_COMMON)

TARGET_TEST_PARMS = \
	-v \
	> test1.txt

SYMBOLS =


default: all >test2.txt


all: clean test_only build program programcommit

notest: clean build program programnotestcommit

dev: clean test_only build commit

test: clean_test test_only

test_only:
	gcc $(CFLAGS) $(INC_TEST) $(SYMBOLS) $(SRC_TEST) -o $(TARGET_TEST) $(MISC_TEST)
	$(PATH_BASE)/test/bin/run_test.bat #in lieu of running the target directly
	

commit:
	$(PATH_BASE)/commit.bat

programcommit:
	$(PATH_BASE)/program_commit.bat output.txt

programnotestcommit:
	$(PATH_BASE)/program_no_test_commit.bat

build:
	avr-gcc -g -Os -mmcu=atmega32 $(CFLAGS) $(INC_AVR) $(SRC_AVR) -o $(TARGET_ELF) $(LDFLAGS_AVR)
	avr-objcopy -j .text -j .data -O ihex $(TARGET_ELF) $(TARGET_HEX)

clean_avr:
	rm -f $(TARGET_ELF) $(TARGET_HEX)

clean_test:
	rm -f $(TARGET_TEST)

clean: clean_test clean_avr

program: 
	avrdude -p atmega32 -c avr109 -P com6 -b 115200 -u -U flash:w:$(TARGET_HEX):i
