#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Command *parse_command(const char *input)
{
    Command *cmd = malloc(sizeof(Command));

    if (cmd == NULL)
        return NULL;

    // Initialize all fields
    cmd->argc = 0;
    cmd->argv = NULL;

    cmd->input_file = NULL;

    cmd->output_file = NULL;
    cmd->output_append = 0;

    cmd->error_file = NULL;
    cmd->error_append = 0;

    const char *p = input;

    while (*p) {

        while (isspace((unsigned char)*p))
            p++;

        if (*p == '\0')
            break;

        const char *start = p;

        while (*p && !isspace((unsigned char)*p))
            p++;

        size_t len = p - start;

        char *arg = malloc(len + 1);

        if (arg == NULL) {
            free_command(cmd);
            return NULL;
        }

        memcpy(arg, start, len);
        arg[len] = '\0';


        char **new_argv = realloc(cmd->argv,
                                  (cmd->argc + 2) * sizeof(char *));

        if (new_argv == NULL) {
            free(arg);
            free_command(cmd);
            return NULL;
        }

        cmd->argv = new_argv;

        cmd->argv[cmd->argc] = arg;
        cmd->argc++;

        cmd->argv[cmd->argc] = NULL;
    }

    return cmd;
}


void free_command(Command *cmd)
{
    if (cmd == NULL)
        return;

    for (int i = 0; i < cmd->argc; i++)
        free(cmd->argv[i]);

    free(cmd->argv);

    free(cmd->input_file);
    free(cmd->output_file);
    free(cmd->error_file);

    free(cmd);
}

