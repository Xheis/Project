#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // atoi

#include <ctype.h>
#include <string.h>
#include <inttypes.h> // For PRIxx and SCNxx macros
#include <avr/pgmspace.h>

#include "cmd_line_buffer.h"
#include "potentiometer.h"
#include "encoder.h"
#include "led.h"
#include "stepper.h"
#include "cmd_parser.h"


static void _cmd_help(void);
static void _cmd_test_stepper(void);
void _cmd_unknown(char*);
void _cmd_enc(char*);
void _cmd_empty(char*);


/* Command table code inspired by Mark McCurry here: http://fundamental-code.com/ on 14/10/17 */
typedef struct {
    const char *nameOfFunction;
    //void (*func)(char*);
    void (*func)(char*);
} commands_t;

int MaxCommandLength = 4;
commands_t commandTable[] = {{"", _cmd_empty},
                             {"help", _cmd_help},
                             {"enc", _cmd_enc},
                             {"testing_shit", _cmd_test_stepper}};

void cmd_parse(const char * cmd)
{
    if (cmd == NULL)
    {
        printf_P(PSTR("ERROR: Tried to parse NULL command pointer\n"));
    }
    else
    {
        uint8_t lengthOfCommandTable = 3;
        for (uint8_t i = 0; i < lengthOfCommandTable; i++)
        {
            //Check where the space is, so we can have different length commands
            int spaceFound = MaxCommandLength;
            if (strchr(cmd,' '))
            {
                spaceFound = strchr(cmd,' ') - cmd; //bloody pointer math.

                //printf_P(PSTR("Space at %" PRId32 "\n"), spaceFound);
            }

            if(!strncmp(cmd,commandTable[i].nameOfFunction,spaceFound)) 
            {
                char arg[80];
                if(sscanf(cmd+3,"%s",&arg) > 0) //anything after our "command word", is a argument
                {
                    commandTable[i].func(arg);
                }
                commandTable[i].func("");   
                return; 
            }
        }
        //OTHERWISE
        printf_P(PSTR("Unknown command: \"%s\"\n"), cmd);
    }
}
/*{
    if (cmd == NULL)
    {
        printf_P(PSTR("ERROR: Tried to parse NULL command pointer\n"));
    }
    else if (*cmd == '\0') // Empty command string
    {
        return;
    }
    else if (!strncmp_P(cmd, PSTR("help"), 4))
    {
        _cmd_help();
    }
    else if (!strncmp_P(cmd, PSTR("pot "), 4))
    {
        //If you get into this far
        printf_P(PSTR("pot: invalid argument \"%s\", syntax is: pot\n"),cmd+4);
    }
    else if (!strncmp_P(cmd, PSTR("test_stepper"), 4))
    {
        //If you get into this far
        printf_P(PSTR("Testing stepper: A restart is needed to quit out of this\n"));
        test_stepper();
    }
    else if (!strncmp_P(cmd, PSTR("pot"), 3))
    {
        printf_P(PSTR("Potentiometer ADC value is %" PRIu16 "\n"), pot_get_value());
    }
    else if (!strncmp_P(cmd, PSTR("enc "), 4))
    {
        //If you get into this far
        if (!strcmp("reset",cmd+4))
        {
            encoder_set_count(0);
            printf_P(PSTR("Encoder count reset to 0\n"));
        }
        else
        {
            //what is this get outta here wtf ya doing
            printf_P(PSTR("enc: invalid argument \"%s\", syntax is: enc [reset]\n"),cmd+4);
        }
    }
    else if (!strncmp_P(cmd, PSTR("enc"), 3))
    {
        printf_P(PSTR("Encoder count is %" PRId32 "\n"), encoder_get_count());
    }
    else
    {
        printf("Unknown command: \"%s\"\n", cmd);
    }
} */
/*{
    int lengthOfCommandTable = sizeof(commandTable)/sizeof(commandTable[0]);
    for (int i = 0; i < lengthOfCommandTable; i++)
    {
        if(!strncmp(cmd,commandTable[i].nameOfFunction,MaxCommandLength)) 
        {
            char* arg;
            if(sscanf(cmd+MaxCommandLength,"%s",&arg) > 0) //anything after our "command word", is a argument
                //strip arg of any excess here
                commandTable[i].func(arg);
            return;
        }
    }
} */
/*
{
    if (cmd == NULL)
    {
        printf_P(PSTR("ERROR: Tried to parse NULL command pointer\n"));
    }
    else if (*cmd == '\0') // Empty command string
    {
        return;
    }
    else if (!strncmp_P(cmd, PSTR("help"), 4))
    {
        _cmd_help();
    }
    else if (!strncmp_P(cmd, PSTR("pot "), 4))
    {
        //If you get into this far
        printf_P(PSTR("pot: invalid argument \"%s\", syntax is: pot\n"),cmd+4);
    }
    else if (!strncmp_P(cmd, PSTR("test_stepper"), 4))
    {
        //If you get into this far
        printf_P(PSTR("Testing stepper: A restart is needed to quit out of this\n"));
        test_stepper();
    }
    else if (!strncmp_P(cmd, PSTR("pot"), 3))
    {
        printf_P(PSTR("Potentiometer ADC value is %" PRIu16 "\n"), pot_get_value());
    }
    else if (!strncmp_P(cmd, PSTR("enc "), 4))
    {
        //If you get into this far
        if (!strcmp("reset",cmd+4))
        {
            encoder_set_count(0);
            printf_P(PSTR("Encoder count reset to 0\n"));
        }
        else
        {
            //what is this get outta here wtf ya doing
            printf_P(PSTR("enc: invalid argument \"%s\", syntax is: enc [reset]\n"),cmd+4);
        }
    }
    else if (!strncmp_P(cmd, PSTR("enc"), 3))
    {
        printf_P(PSTR("Encoder count is %" PRId32 "\n"), encoder_get_count());
    }
    else if (!strncmp_P(cmd, PSTR("led on"), 6))
    {
        led_on();
        printf_P(PSTR("LED is on\n"));
    }
    else if (!strncmp_P(cmd, PSTR("led off"), 7))
    {
        led_off();
        printf_P(PSTR("LED is off\n"));
    }
    else if (!strncmp_P(cmd, PSTR("led "), 4))
    {
        int16_t led = 0;
        uint8_t EOF_Found = 0;
        uint8_t Letters_Found = 0;
        uint8_t Blank_Found = 0;

        //uint8_t Negative = 0;
        //check it's not an overflow, now that there's much we can do
        for(int index = 4; index < 8; index++) {
            
            //if(*(cmd+index) == '\n')
            if((cmd[index]) == '\0')
            {
                if (index == 4)
                {
                    //blank entry
                    Blank_Found++;
                    break;
                }
                //EOF found
                EOF_Found++;
                break;
            }
            if (cmd[index] < 48|| cmd[index] > 57)
            {
                // NOn nUmber Shit GEETTIng all up here
                Letters_Found++;
            }
        }

        led = atol(cmd + 4);
        if (led > 255)
        {
            led = 255;
        }
        else if (led < 0)
        {
            led = 0;
        }

        if (EOF_Found)
        {
            //No overflow, not negative so let's continue
            //It's a number. let's set the LED-var to it
            
            led_set_brightness(led);
            printf_P(PSTR("LED brightness set to %" PRIu8 "\n"), led);
        }
        else if (Letters_Found)
        {
            printf_P(PSTR("led: invalid argument \"%s\", syntax is: led [on|off|<value>]\n"), cmd+4);

        }
        else if(Blank_Found)
        {
            //printf_P(PSTR("LED brightness is\"%u\"\n"), led_get_brightness());
            printf_P(PSTR("LED brightness is %" PRIu8 "\n"), led_get_brightness());
        }
        else
        {
            led = 255; //0
            led_set_brightness(led);
            printf_P(PSTR("LED brightness set to %" PRIu8 "\n"), led); //printf_P(PSTR("Overflowed! LED turning off %" PRIu8 "\n"), led);
        }



    }
    else if (!strncmp_P(cmd, PSTR("led"), 3))
    {
        printf_P(PSTR("LED brightness is %" PRIu8 "\n"), led_get_brightness());
    }
    else
    {
        printf("Unknown command: \"%s\"\n", cmd);
    }
}
*/
    
void _cmd_help(void)
{
    printf_P(PSTR(
        "\n"
        "\n"
    ));
   
    printf_P(PSTR("\n"));

    // Describe argument syntax using POSIX.1-2008 convention
    // see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap12.html
    printf_P(PSTR(
        "Available commands:\n"
        "    help                       Print this help message\n"
        "    pot                        Get potentiometer ADC value (0-1023)\n"
        "    enc [reset]                Get or reset encoder count\n"
        "    led [on|off|<value>]       Get or set LED brightness (0-255)\n"
        "    test_stepper               A restart is needed to quit out of this\n"
        " --------------------------------------------------------------- "
        "\nC3206246 BUILD DATE: "__DATE__" "__TIME__"\n\n"
        "\n"
    ));
}
void _cmd_enc(char* arg)
{
    if (!strcmp("reset",arg))
    {
        encoder_set_count(0);
        printf_P(PSTR("Encoder count reset to 0\n"));
    }
    else if (!strcmp("",arg))
    {
        printf_P(PSTR("Encoder count is %" PRId32 "\n"), encoder_get_count());
    }
    else
    {
        printf_P(PSTR("enc: invalid argument \"%s\", syntax is: enc [reset]\n"),arg);
    }
}
void _cmd_test_stepper(void) { return 0; }
void _cmd_unknown(char* arg)
{  
    printf_P(PSTR("Unknown command: \"%s\"\n"),arg); 
}
void _cmd_empty(char* arg)
{  
    printf_P(PSTR("Unknown command: \"%s\"\n"),arg); 
}