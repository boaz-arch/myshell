#include "redirection.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int apply_redirections(Command *cmd)
{
    if (cmd == NULL || cmd->argv == NULL)
        return -1;

    char **new_argv = malloc((cmd->argc + 1) * sizeof(char *));
    if (new_argv == NULL)
        return -1;

    int new_argc = 0;

    for (int i = 0; i < cmd->argc; i++) {

        if (strcmp(cmd->argv[i], ">") == 0) {

            // Need a filename after >
            if (i + 1 >= cmd->argc) {
                fprintf(stderr, "syntax error: expected file after >\n");
                free(new_argv);
                return -1;
            }

            cmd->output_file = strdup(cmd->argv[i + 1]);

            if (cmd->output_file == NULL) {
                free(new_argv);
                return -1;
            }

            cmd->output_append = 0;

            i++; // skip filename
            continue;
        }

        new_argv[new_argc] = cmd->argv[i];
        new_argc++;
    }

    new_argv[new_argc] = NULL;

    free(cmd->argv);

    cmd->argv = new_argv;
    cmd->argc = new_argc;

    return 0;
}

