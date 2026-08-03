#include "shell.h"
#include "parser.h"
#include "executor.h"
#include "history.h"
#include "pipeline.h"
#include "jobs.h"
#include "signals.h"

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


void sigchld_handler(int sig) {
    (void)sig;

    pid_t pid;

    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        mark_job_finished(pid);
    }
}

void sigint_handler(int sig) {
    (void)sig;
    
    if (foreground_pid > 0){
        kill(foreground_pid, SIGINT);
    }
}


void print_prompt() {
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        printf("MyShell%s>> ", cwd);
    } else {
        printf("MyShell>> ");
    }
    
    fflush(stdout);
}

int read_input(char *input, size_t size) {
    if (fgets(input, size, stdin) == NULL)
        return 0;               

    input[strcspn(input, "\n")] = '\0';

    return 1;
}


void shell_loop(void) {
    char input[MAX_INPUT_SIZE];
    Command *cmd;

    while (1) {
        jobs_check_finished();
        print_prompt();

        if (!read_input(input, sizeof(input))) {
            printf("\n");
            break;
        }

        if (input[0] == '\0')
            continue;
            
        history_add(input);
        
        Pipeline *pipeline = parse_pipeline(input);
        
        if (pipeline != NULL) {
            execute_pipeline(pipeline);
            free_pipeline(pipeline);
            
        } else {
            cmd = parse_command(input);
            
            if (cmd == NULL) {
                fprintf(stderr, "parse failed\n");
                continue;
            }
            
            execute_command(cmd);
            free_command(cmd);
            
        }
        
        jobs_check_finished();

    }
}


int main() {

    struct sigaction sa_chld;
    
    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa_chld, NULL);
    
    struct sigaction sa_int;
    
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa_int, NULL);
    
    
    history_init();
    jobs_init();
    shell_loop();
    history_cleanup();
    
    return 0;
}
