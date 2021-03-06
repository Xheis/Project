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
void _cmd_move_set_steps(char*);
void _cmd_log(char*);
void _cmd_sin(char*);
void _cmd_set(char*);
void _cmd_get(char*);
void _cmd_ctrl(void);

void _cmd_test_step_params(char*);

//test_step_params
static float x;
static float y;
static float VRef;
static float V;
static float Theta;
static float Rho;
static float b;

void setX(float tempX) { x = tempX; }
void setY(float tempY) { y = tempY; }
void setVRef(float tempVRef) { VRef = tempVRef; }
void setV(float tempV) { V = tempV; }
void setTheta(float tempTheta) { Theta = tempTheta; }
void setRho(float tempRho) { Rho = tempRho; }
void setB(float tempB) { b = tempB; }
float getX(void) { return x; }
float getY(void) { return y; }
float getVRef(void) { return VRef; }
float getV(void) { return V; }
float getTheta(void) { return Theta; }
float getRho(void) { return Rho; }
float getB(void) { return b; }

/* Command table code Adapted from: http://fundamental-code.com/ on 14/10/17, under MIT License use*/
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
                             //{"set_velocity", _cmd_set_velocity, "set_velocity m/s"},
                             //{"set_dir", _cmd_set_dir, "set_dir [forwards|backwards|left|right]"},
                             {"move_set_steps", _cmd_move_set_steps, "move_set_steps steps"},
                             {"log", _cmd_log, "log [enc] <samples>"},
                             {"sin", _cmd_sin, "sin <# Args> Args[]"},
                             {"set", _cmd_set, "set [theta|pho] <value>"},
                             {"get", _cmd_get, "get [theta|pho]"},
                             {"ctrl", _cmd_ctrl, "ctrl - returns next step in controller"},
                             {"test_step_params", _cmd_test_step_params, "runs a stepper test suite"},
                             {"testing_stepper", _cmd_test_stepper, "test_step_params(int step_Speed, int step_distance, int step_inbetween_delay, int dir)"}};

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
        		printf_P(PSTR("Command: %s | SpaceFound: %d | "), cmd, spaceFound);
                if(strcpy(&arg, cmd+spaceFound+1)) //if(sscanf(cmd+spaceFound,"*",&arg) > 0) //anything after our "command word", is a argument
                {
                	printf_P(PSTR("Arg: %s\n"), arg);
                    commandTable[i].func(arg);
                }
                else
                {
					printf_P(PSTR("No Arg \n"));
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
    printf_P(PSTR("Distance parsed in as a char was %s \n"), Distance_mm);

    printf_P(PSTR("Distance atoi'd is %d \n"), atoi(Distance_mm));
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

void _cmd_move_set_steps(char* steps)
{
    printf_P(PSTR("OKAY\n"));
    move_set_steps(atoi(steps));
    printf_P(PSTR("DONE\n"));
}

void _cmd_log(char* arg)
{
    printf_P(PSTR("OKAY, %s \n"),arg);
    //create the format of argc & argv[] log wants
    char* argv[2] = {"log","enc","10"};
    log_cmd(3,argv);
    // int argc = 3;
    // char* argv[3];
    // argv[0] = "log";
    // argv[1] = strtok (arg," ");
    // argv[2] = strtok (NULL," ");
    // printf_P(PSTR("TEST argc = %c, arg[0]=%s, arg[1]=%s, arg[2]=%s\n"),argc,argv[0],argv[1],argv[2]);

    // log_cmd(3,argv);
    // printf_P(PSTR("DONE argc = %d, arg[0]=%s, arg[1]=%s, arg[2]=%s\n"),argc,argv[0],argv[1],argv[2]);
}

void _cmd_sin(char* arg)
{
    //delimit into array
    int i;
    char *p = strtok (arg, " "); //delimit shit by space
    char *array[80]; //we know that it'll take the format "sin", "10", "1.0", "5.0", and we've hardcoded 80

    while (p != NULL)
    {
        array[i] = p;
        p = strtok (NULL, " "); //return the next string
        i++; //incriment through the array
    }

    array[i] = NULL;

    printf_P(PSTR("OKAY %s\n"),arg);
    sin_table_cmd(i, array);
    printf_P(PSTR("DONE\n"));
}

void _cmd_set(char* cmd)
{
	printf_P(PSTR("OKAY\n"));
    printf_P(PSTR("Parsed char was %s \n"), cmd);
    char * varSelect;
    char * setValue;
    uint8_t EOF_Found = 0;
    uint8_t f_index = 0;
    uint8_t Blank_Found = 0;
    //uint8_t Negative = 0;
    //check it's not an overflow, now that there's much we can do
    for(int index = 4; index < 8; index++) 
    {
        
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
    }


    varSelect = (cmd);
    varSelect[1] = '\0'; //terminate the string early, 'cause we're shit.
    setValue = (cmd+2);

    if (!strncmp_P(varSelect, PSTR("x"), 1))
    {
       /* code */
        setX(atof(setValue));//x = atof(setValue);

    } 
    else if (!strncmp_P(varSelect, PSTR("y"), 1))
    {
       /* code */
        //y = atof(setValue);
        setY(atof(setValue));
    }     
    else if (!strncmp_P(varSelect, PSTR("vref"), 1))
    {
        setVRef(atof(setValue));
    }    
    else if (!strncmp_P(varSelect, PSTR("v"), 1))
    {
        setV(atof(setValue));
    }    
    else if (!strncmp_P(varSelect, PSTR("theta"), 1))
    {
    	setValue = (setValue+4);
       	printf_P(PSTR("OKAY, %S found. Setting it from %s to %f \n"),varSelect,setValue,atof(setValue));
        setTheta(atof(setValue));
    }           
    else if (!strncmp_P(varSelect, PSTR("rho"), 1))
    {
        setRho(atof(setValue));
    }                
    else if (!strncmp_P(varSelect, PSTR("b"), 1))
    {
        setB(atof(setValue));
    }                    
    else if (!strncmp_P(varSelect, PSTR("dir"), 1))
    {
    	setValue = (setValue+2);
        set_dir(setValue);
    }                     
    else if (!strncmp_P(varSelect, PSTR("velocity"), 1))
    {
    	setValue = (setValue+7);
        set_velocity(atof(setValue));
    }    
    else
    {
    //shit
    }
}

void _cmd_get(char* cmd)
{

	printf_P(PSTR("OKAY\n"));
    printf_P(PSTR("Parsed char was %s \n"), cmd);
     char * varSelect;

    varSelect = cmd;
    //varSelect[1] = '\0'; //terminate the string early, 'cause we're shit.


        if (!strncmp_P(varSelect, PSTR("x"), 1))
       {
            printf_P(PSTR("%s is %f\n"), varSelect, getX());
       } 
        else if (!strncmp_P(varSelect, PSTR("y"), 1))
       {
            printf_P(PSTR("%s is %f\n"), varSelect, getY());
       }  
        else if (!strncmp_P(varSelect, PSTR("vref"), 1))
       {
            printf_P(PSTR("%s is %f\n"), varSelect, getVRef());
       }    
        else if (!strncmp_P(varSelect, PSTR("v"), 1))
       {
            printf_P(PSTR("%s is %f\n"), varSelect, getV());
       }    
        else if (!strncmp_P(varSelect, PSTR("theta"), 1))
       {
            printf_P(PSTR("%s is %f\n"), varSelect, getTheta());
       }     
        else if (!strncmp_P(varSelect, PSTR("Rho"), 1))
       {
            printf_P(PSTR("%s is %f\n"), varSelect, getRho());
       }   
       else if (!strncmp_P(varSelect, PSTR("b"), 1))
       {
            printf_P(PSTR("%s is %f\n"), varSelect, getB());
       }    
       else
       {
        //shit
       	printf_P(PSTR("SHIT; x was %f \n"),getX());
       }
}


void _cmd_ctrl()
{
	printf_P(PSTR("%f\n"), ctrl_run(getY(),getTheta(),getV()));
}

void _cmd_test_step_params(char* arg) //int step_Speed, int step_distance, int step_inbetween_delay, int dir);
{

    // int argc = 3;
     char* argv[3];
    argv[0] = strtok (arg," ");
    argv[1] = strtok (NULL," ");
    argv[2] = strtok (NULL," ");
    argv[3] = strtok (NULL," ");
    printf_P(PSTR("TEST arg[0]=%s, arg[1]=%s, arg[2]=%s (%f), arg[3]=%s\n"),argv[0],argv[1],argv[2],atof(argv[2]),argv[3]);
    test_step_params(atoi(argv[0]), atoi(argv[1]), atof(argv[2]), atoi(argv[3]));
}