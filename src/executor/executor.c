#include "executor.h"
#include "builtin.h"
#include "redirection.h"
#include "parser.h"
#include "jobs.h"
#include "signals.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>


int execute_command(Command *cmd) {
    if (cmd == NULL || cmd->argc == 0)
        return 0;

    if (is_builtin(cmd)) {
        return execute_builtin(cmd);
    }
  
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }
    

    if (pid == 0) {            
    
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
                
        if (apply_redirections(cmd) < 0)
            _exit(EXIT_FAILURE);
            
        execvp(cmd->argv[0], cmd->argv);
        
        perror("execvp");
        _exit(EXIT_FAILURE);
    }
    
    int status;
    
    if(cmd->background){
        char command[256] = "";
        
        for (int i = 0; i < cmd->argc; i++){
            strcat(command, cmd->argv[i]);
            
            if (i < cmd->argc -1){
                strcat(command, " ");
            }
        }
        
        int job_id = add_job(pid, command);
        
        printf("[%d] %d\n", job_id, pid);
        return 0;
    }
    
    foreground_pid = pid;

    if (waitpid(pid, &status, WUNTRACED) < 0) {
        foreground_pid = 0;
        perror("waitpid");
        return -1;
    }
    
    foreground_pid = 0;
    
    if (WIFSTOPPED(status)) {
        char command[256] = "";
        
        
        for (int i = 0; i < cmd->argc; i++){
            strcat(command, cmd->argv[i]);
            
            if (i < cmd->argc -1){
                strcat(command, " ");
            }
        }
        
        int job_id = add_job(pid, command);
    
        mark_job_stopped(pid);
        
        printf("[%d] Stopped                      %s\n", job_id, command);
        
        return 0;
    }

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return -1;

}

