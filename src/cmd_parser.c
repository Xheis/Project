#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // atoi

#include <ctype.h>
#include <string.h>
#include <inttypes.h> // For PRIxx and SCNxx macros
#include <avr/pgmspace.h>
#include <util/delay.h>

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
void _cmd_move_set_dist(char*);
// void _cmd_move_set_dist(int,int);
void _cmd_move_set_time(char*);
void _cmd_set_velocity(char*);
void _cmd_set_dir(char*);

/* Command table code inspired by Mark McCurry here: http://fundamental-code.com/ on 14/10/17 */
typedef struct {
    const char *nameOfFunction;
    void (*func)(char*);
    const char *helpText;
} commands_t;

int MaxCommandLength = 20;
commands_t commandTable[] = {{"", _cmd_empty, ""},
                             {"help", _cmd_help, "Gives all commands"},
                             {"enc", _cmd_enc, "Gets and sets enc value"},
                             {"move_set_dist", _cmd_move_set_dist, "move_set_dist Distance_mm [stepper]"},
                             {"move_set_time", _cmd_move_set_time, "move_set_time seconds"},
                             {"set_velocity", _cmd_set_velocity, "set_velocity m/s"},
                             {"set_dir", _cmd_set_dir, "set_dir [forwards|backwards|left|right]"},
                             {"testing_shit", _cmd_test_stepper, "runs a stepper test suite"}};


void cmd_parse(const char * cmd)
{
    if (cmd == NULL)
    {
        printf_P(PSTR("ERROR: Tried to parse NULL command pointer\n"));
    }
    else
    {
        uint8_t lengthOfCommandTable = sizeof commandTable / sizeof commandTable[0];
        for (uint8_t i = 0; i < lengthOfCommandTable; i++)
        {
            //Check where the space is, so we can have different length commands
            int spaceFound = MaxCommandLength;
            if (strchr(cmd,' '))
            {
                spaceFound = strchr(cmd,' ') - cmd; //bloody pointer math.

                //printf_P(PSTR("Space at %" PRId32 "\n"), spaceFound);
            }
            else if (strchr(cmd,'\n'))
            {
                spaceFound = strchr(cmd,'\n') - cmd; //bloody pointer math.

                //printf_P(PSTR("Space at %" PRId32 "\n"), spaceFound);
            }

            if(!strncmp(cmd,commandTable[i].nameOfFunction,spaceFound)) 
            {
                char arg[80];
                if(sscanf(cmd+spaceFound,"%s",&arg) > 0) //anything after our "command word", is a argument
                {
                    commandTable[i].func(arg);
                }
                else
                {
                commandTable[i].func("");   
                }
                return; 
            }
        }
        //OTHERWISE
        printf_P(PSTR("Unknown command: \"%s\"\n"), cmd);
    }
}

    
void _cmd_help(void)
{
    printf_P(PSTR(" --------------------------------------------------------------- "
        "\nC3206246 BUILD DATE: "__DATE__" "__TIME__"\n\n"
        "\n"
    ));
    printf_P(PSTR("Available commands:\n"));

    int numberOfCommands = 0;
    numberOfCommands = sizeof commandTable / sizeof commandTable[0];
    for (int i = 0; i < numberOfCommands; ++i)
    {
        /* code */
        printf_P(PSTR("| %s \t\t\t\t %s \n"),commandTable[i].nameOfFunction,commandTable[i].helpText);
        _delay_ms(100);
    }
    printf_P(PSTR(" --------------------------------------------------------------- \n"));

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

// COMMANDS ------------------------------------------------------------------------------
void _cmd_test_stepper(void) { test_stepper(); }
void _cmd_unknown(char* arg)
{  
    printf_P(PSTR("Unknown command: \"%s\"\n"),arg); 
}
void _cmd_empty(char* arg)
{  
    printf_P(PSTR("Unknown command: \"%s\"\n"),arg); 
}
void _cmd_move_set_dist(char* Distance_mm) 
{
    printf_P(PSTR("OKAY\n"));
    move_set_dist(atoi(Distance_mm));
    printf_P(PSTR("DONE\n"));
}
// void _cmd_move_set_dist(int Distance_mm,int stepper)
// {
//     move_set_dist(Distance_mm, stepper);
// }
void _cmd_move_set_time(char* time_in_sec) 
{
    printf_P(PSTR("OKAY\n"));
    move_set_time(atoi(time_in_sec));
    printf_P(PSTR("DONE\n"));
}
void _cmd_set_velocity(char* velocity)
{
    printf_P(PSTR("OKAY\n"));
    set_velocity(atof(velocity));
    printf_P(PSTR("DONE\n"));
}
void _cmd_set_dir(char* dir)
{
    printf_P(PSTR("OKAY\n"));
    set_dir(dir);
    printf_P(PSTR("DONE\n"));
}
