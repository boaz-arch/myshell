#include "pipeline.h"
#include "redirection.h"
#include "parser.h"
#include "jobs.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int execute_pipeline(Pipeline *pipeline)
{
    int count = pipeline->count;
    Command **commands = pipeline->commands;

    if (count <= 0)
        return -1;

    int pipes[count - 1][2];
    pid_t pids[count];

    // Create pipes
    for (int i = 0; i < count - 1; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            return -1;
        }
    }

    // Create processes
    for (int i = 0; i < count; i++) {

        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork");
            return -1;
        }


        if (pids[i] == 0) {

            // Connect stdin to previous pipe
            if (i > 0) {
                if (dup2(pipes[i - 1][0], STDIN_FILENO) < 0) {
                    perror("dup2 stdin");
                    _exit(EXIT_FAILURE);
                }
            }


            // Connect stdout to next pipe
            if (i < count - 1) {
                if (dup2(pipes[i][1], STDOUT_FILENO) < 0) {
                    perror("dup2 stdout");
                    _exit(EXIT_FAILURE);
                }
            }


            // Child no longer needs pipe descriptors
            for (int j = 0; j < count - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }


            // Apply redirections (<, >, >>)
            if (apply_redirections(commands[i]) < 0)
                _exit(EXIT_FAILURE);


            execvp(commands[i]->argv[0], commands[i]->argv);

            perror("execvp");
            _exit(EXIT_FAILURE);
        }
    }


    // Parent closes pipes
    for (int i = 0; i < count - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    if (commands[count-1]->background){
        printf("{%d}\n", pids[count-1]);
        return 0;
    }

    // Wait for all commands
    for (int i = 0; i < count; i++) {
        waitpid(pids[i], NULL, 0);
    }


    return 0;
}

