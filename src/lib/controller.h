#ifndef CONTROLLER_H
#define CONTROLLER_H

enum {
    CTRL_N_INPUT  = 3, // number of reference signals and plant states : REF*,v,Theta
    CTRL_N_STATE  = 1, // number of controller states : 1, just cause
    CTRL_N_OUTPUT = 1, // number of controller outputs : F
};

void ctrl_init(void);
void ctrl_set_state(const float state[CTRL_N_STATE]);
float * ctrl_get_state(void);
float * ctrl_run(const float input[CTRL_N_INPUT]);

#endif