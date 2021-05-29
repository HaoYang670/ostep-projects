/**
 * Basic shell
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include "linked_list.c"
#include "parameter.c"

node* paths = NULL;

void print_error(){
    fprintf(stderr, "An error has occurred\n");
}
void print_prompt(){
    printf("wish> ");
}
/**
 * Build in command
 */
node* change_path(parameter* p){
    assert (strcmp(p->argv[0], "path") == 0);
    free_list(paths);
    paths = NULL;

    for(int i=1; i<p->argc;  i++) {
        paths = add_node(p->argv[i], paths);
    }
    return paths;
}

void cd(parameter* p){
    assert (strcmp(p->argv[0], "cd") == 0);
    if(p->argc != 2 || chdir(p->argv[1]) != 0) print_error();
}


/**
 * This function will never return.
 */
void execute(parameter* p){
    node* dir = paths;
    while(dir != NULL){
        // generate directory + command_name
        int len = strlen(dir->content)+strlen(p->argv[0]);
        char file_name[len];
        snprintf(file_name, len+2, "%s/%s", dir->content, p->argv[0]);

        if(access(file_name, X_OK) == 0) execv(file_name, p->argv);
        else dir = dir->next;
    }
    print_error();
    exit(1);
}

/**
 * return true if redirect successfully or no redirectory
 * return false if error occurs
 */
bool redirect(parameter* p){
    int idx = 0;
    for(; idx<p->argc; idx++){
        if(strcmp(p->argv[idx], ">") == 0) break;
    }

    if(idx == p->argc) return true; // no rediction, no error
    else if(idx == 0 || idx != p->argc - 2) { // rediction with no cmd before or  0 or > 1 output file
        print_error();
        return false;
    }
    else {
        int fd = open(p->argv[idx+1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if(fd == -1){
            print_error();
            return false;
        }
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        p->argv[idx] = NULL;
        p->argv[idx+1] = NULL;
        p->argc -= 2;
    }
    return true;
}

void analyse(commands* cmds){
    // execute all commends
    for(int i=0; i<cmds->num; i++){
        parameter* p = cmds->parameters[i];
        if(p->argc <= 0) continue;
        if(!redirect(p)) continue;
        else if(strcmp(p->argv[0], "path") == 0)       change_path(p);
        else if(strcmp(p->argv[0], "cd") == 0)    cd(p);
        else if (strcmp(p->argv[0], "exit") == 0) {
            if(p->argc == 1) exit(0);
            else print_error();
        }
        else { // create a child process to execute the command
            int pid = fork();
            if(pid == 0) execute(p); // execute() should never return
        }
    }
    while(wait(NULL) != -1);
}

commands* parse(char* string){
    // store the pointer of all concurrent commands
    parameter* cmds[strlen(string)];
    int cmd_num = 0;
    for(int i=0; i<strlen(string); i++){
        cmds[i] = NULL;
    } 
    // parse concurrent commands
    while(string != NULL){
        char* next_cmd = strsep(&string, "&\n");

        if(strlen(next_cmd) > 0){
            parameter* p = initial_parameter(strlen(next_cmd));
            // parse each command
            while(next_cmd != NULL){
                char* next_argv = strsep(&next_cmd, " \n");
                if(strlen(next_argv) > 0) p->argv[p->argc++] = next_argv;
            }
            cmds[cmd_num++] = p;
        }
    }
    return change_to_commands(cmd_num, cmds);
}

int main(int argc, char *argv[]) {
    int STDIN = dup(STDIN_FILENO);
    int STDOUT = dup(STDOUT_FILENO);
    int STDERR = dup(STDERR_FILENO);

    paths = add_node("/bin", NULL);
    // read commands from a batch file
    // change directory and input
    bool batch_mode = false;
    if(argc > 2){
        print_error();
        exit(1);
    }
    else if(argc == 2){
        batch_mode = true;
        int fd = open(argv[1], O_RDONLY);
        if(fd == -1){
            print_error();
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
    }

    // read command from input and execute
    // loop and loop ...
    while (true) {
        dup2(STDOUT, STDOUT_FILENO);
        dup2(STDERR, STDERR_FILENO);

        if(!batch_mode) print_prompt();
        // read command from input
        char* user_input = NULL; 
        size_t len = 0;
        if((getline(&user_input, &len, stdin)) == -1) break;

        // execute the command
        commands* cmds = parse(user_input);
        analyse(cmds);
        free_commands(cmds);
    }
    return 0;
}
