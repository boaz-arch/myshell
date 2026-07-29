#include "executor.h"
#include "builtin.h"
#include "redirection.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


int execute_command(Command *cmd)
{
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

        // Apply redirections in child
        if (apply_redirections(cmd) < 0) {
            exit(EXIT_FAILURE);
        }

        // Output redirection
        if (cmd->output_file != NULL) {

            int flags = O_WRONLY | O_CREAT;

            if (cmd->output_append)
                flags |= O_APPEND;
            else
                flags |= O_TRUNC;

            int fd = open(cmd->output_file, flags, 0644);

            if (fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }


        execvp(cmd->argv[0], cmd->argv);

        perror("execvp");
        exit(EXIT_FAILURE);
    }


    int status;

    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return -1;
    }

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return -1;
}

