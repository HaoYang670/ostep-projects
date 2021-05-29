#include <stdlib.h>

/**
 * data structure to store one parsed command
 */
typedef struct parameter {
    char **argv;
    int argc;
} parameter;

/**
 * data structure to store all parsed command
 */
typedef struct commands {
    parameter **parameters;
    int num;
} commands;


parameter* initial_parameter(int max_len){
    parameter* p = (parameter*)malloc(sizeof(parameter));

    p->argv = (char**) malloc(sizeof(char*)*max_len);
    for(int i=0; i<max_len; i++){
        p->argv[i] = NULL;
    }

    p->argc = 0;
    return p;
}

void free_parameter(parameter* p){
    free(p->argv);
    free(p);
}

commands* initial_commands(int num){
    commands *c = (commands*) malloc(sizeof(commands));
    c->num = num;
    c->parameters = (parameter**)malloc(sizeof(parameter*));
    for(int i=0; i<num; i++){
        c->parameters[i] = NULL;
    }
    return c;
}

commands* change_to_commands(int num, parameter **vals){
    commands* c = initial_commands(num);
    for(int i=0; i<num; i++){
        c->parameters[i] = vals[i];
    }
    return c;
}

void free_commands(commands* c){
    for(int i=0; i<c->num; i++){
        free_parameter(c->parameters[i]);
    }
    free(c);
}