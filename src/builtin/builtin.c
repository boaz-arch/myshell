#include "builtin.h"
#include "parser.h"
#include "history.h"
#include "jobs.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int is_builtin(const Command *cmd) {
    if (cmd == NULL || cmd->argc == 0)
        return 0;

    if (strcmp(cmd->argv[0], "cd") == 0)
        return 1;
     
    if (strcmp(cmd->argv[0], "pwd") == 0)
        return 1;
        
    if (strcmp(cmd->argv[0], "exit") == 0)
        return 1;
        
    if (strcmp(cmd->argv[0], "history") == 0)
        return 1;
        
    if (strcmp(cmd->argv[0], "jobs") == 0)
        return 1;
        
    

    return 0;
}

int execute_builtin(Command *cmd) {
    if (strcmp(cmd->argv[0], "cd") == 0) {
        const char *path;

        if (cmd->argc == 1) {
            path = getenv("HOME");

            if (path == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
                return 1;
            }
        } else {
            path = cmd->argv[1];
        }

        if (chdir(path) != 0) {
            perror("cd");
            return 1;
        }

        return 0;
    }


    if (strcmp(cmd->argv[0], "pwd") == 0) {
        char cwd[4096];

        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("pwd");
            return 1;
        }

        printf("%s\n", cwd);
        return 0;
    }


    if (strcmp(cmd->argv[0], "exit") == 0) {
        exit(0);
    }


    if (strcmp(cmd->argv[0], "history") == 0) {
        history_print();
        return 0;
    }
    
    if (strcmp(cmd->argv[0], "jobs") == 0){
        jobs_print();
        return 0;
    }

    return -1;
}

