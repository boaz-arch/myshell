#include "pipeline.h"
#include "redirection.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int execute_pipeline(Command *left, Command *right)
{
    int pipefd[2];

    if (pipe(pipefd) < 0) {
        perror("pipe");
        return -1;
    }

    pid_t left_pid = fork();

    if (left_pid < 0) {
        perror("fork");
        return -1;
    }

    if (left_pid == 0) {
        // Left command writes to the pipe
        close(pipefd[0]);

        if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(pipefd[1]);

        if (apply_redirections(left) < 0)
            exit(EXIT_FAILURE);

        execvp(left->argv[0], left->argv);

        perror("execvp");
        exit(EXIT_FAILURE);
    }

    pid_t right_pid = fork();

    if (right_pid < 0) {
        perror("fork");
        return -1;
    }

    if (right_pid == 0) {
        // Right command reads from the pipe
        close(pipefd[1]);

        if (dup2(pipefd[0], STDIN_FILENO) < 0) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(pipefd[0]);

        if (apply_redirections(right) < 0)
            exit(EXIT_FAILURE);

        execvp(right->argv[0], right->argv);

        perror("execvp");
        exit(EXIT_FAILURE);
    }

    // Parent
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, NULL, 0);

    return 0;
}

