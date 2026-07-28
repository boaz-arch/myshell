#include "executor.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int execute_command(Command *cmd)
{
    if (cmd == NULL || cmd->argc == 0)
        return 0;

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // Child process

        execvp(cmd->argv[0], cmd->argv);

        // execvp only returns on error
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    // Parent process
    int status;

    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return -1;
    }
    
    WIFEXITED(status);

    return WEXITSTATUS(status);
}

