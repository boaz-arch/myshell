#include "executor.h"
#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int execute_command(Command *cmd) {
    if (cmd == NULL || cmd->argc == 0)
        return 0;
        
    if (is_builtin(cmd)){
        return execute_builtin(cmd);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {

        execvp(cmd->argv[0], cmd->argv);

        perror("execvp");
        exit(EXIT_FAILURE);
    }

    int status;

    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return -1;
    }
    
    if (WIFEXITED(status)) return WEXITSTATUS(status);
    
    return -1;
}




