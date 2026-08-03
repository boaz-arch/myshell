#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

static char *next_token(const char **p) {
    while (isspace((unsigned char)**p))  (*p)++;  
       
    if (**p == '\0') return NULL;
    
    const char *start = *p;
    
    while (**p && !isspace((unsigned char) **p)) (*p)++;
    
    size_t len = *p - start;
    
    char *token = malloc(len + 1);
    
    if (token == NULL) return NULL;
    
    memcpy(token, start, len);
    token[len] = '\0';
    
    return token;
}


Command *parse_command(const char *input) {

    Command *cmd = malloc(sizeof(Command));
    
    if (cmd == NULL) return NULL;
    
    cmd->argc = 0;
    cmd->argv =NULL;
    
    cmd->input_file = NULL;
    
    cmd->output_file = NULL;
    cmd->output_append = 0;
    
    cmd->error_file = NULL;
    cmd->error_append = 0;
    
    cmd->background = 0;
    
    const char *p = input;
    
    while (1){
        char *token = next_token(&p);
        
        if (token == NULL) break;
        
        if (strcmp(token, ">") == 0){
            free(token);
            
            char *filename = next_token(&p);
            
            if (filename == NULL) {
                fprintf(stderr, "syntax error: expected filename after >\n");
                free_command(cmd);
                return NULL;      
            }
            
            cmd->output_file = filename;
            cmd->output_append = 0;
            
            continue;
        }
        
        if (strcmp(token, ">>") == 0){
            free(token);
            
            char *filename = next_token(&p);
            
            if (filename == NULL) {
                fprintf(stderr, "syntax error: expected filename after >>\n");
                free_command(cmd);
                return NULL;      
            }
            
            cmd->output_file = filename;
            cmd->output_append = 1;
            
            continue;
        }
        
        if (strcmp(token, "<") == 0){
            free(token);
            
            char *filename = next_token(&p);
            
            if (filename == NULL) {
                fprintf(stderr, "syntax error: expected filename after <\n");
                free_command(cmd);
                return NULL;      
            }
            
            cmd->input_file = filename;
            
            continue;
        }
        
        if (strcmp(token, "&") == 0){
            free(token);
            cmd->background = 1;
            continue;
        }
        
        
        char **new_argv = realloc(cmd->argv, (cmd->argc +2) * sizeof(char*));
        
        if (new_argv == NULL){
            free(token);
            free_command(cmd);
            return NULL;
        }
        
        cmd->argv = new_argv;
        cmd->argv[cmd->argc++] = token;
        cmd->argv[cmd->argc] = NULL;
    }
        
    return cmd;
}

Pipeline *parse_pipeline(const char *input) {
    if (strchr(input, '|') == NULL)
        return NULL;
    
    if (input == NULL)
        return NULL;
        
    const char *start = input;
    
    while (*start && isspace((unsigned char) *start))
        start++;
        
    if (*start == '|'){
        fprintf(stderr, "Syntax error near unexpected token '|'\n");
        return NULL;
    }
    
    const char *end = input + strlen(input) - 1;
    
    while (end > input && isspace((unsigned char) *end))
        end--;
        
    if (*end == '|'){
        fprintf(stderr, "Syntax error near unexpected token '|'\n");
        return NULL;
    }

    char *copy = strdup(input);
    if (copy == NULL)
        return NULL;

    Pipeline *pl = malloc(sizeof(Pipeline));
    if (pl == NULL) {
        free(copy);
        return NULL;
    }

    pl->commands = NULL;
    pl->count = 0;

    char *token = strtok(copy, "|");

    while (token != NULL) {

        // Reject empty commands (example: || or | ls)
        int empty = 1;
        for (char *p = token; *p != '\0'; p++) {
            if (!isspace((unsigned char)*p)) {
                empty = 0;
                break;
            }
        }

        if (empty) {
            free_pipeline(pl);
            free(copy);
            return NULL;
        }


        Command *cmd = parse_command(token);

        if (cmd == NULL || cmd->argc == 0) {
            fprintf(stderr, "Syntax error near unexpected token '|'\n");
            free_command(cmd);
            free_pipeline(pl);
            free(copy);
            return NULL;
        }


        Command **new_commands =
            realloc(pl->commands, (pl->count + 1) * sizeof(Command *));

        if (new_commands == NULL) {
            free_command(cmd);
            free_pipeline(pl);
            free(copy);
            return NULL;
        }

        pl->commands = new_commands;
        pl->commands[pl->count] = cmd;
        pl->count++;

        token = strtok(NULL, "|");
    }


    // Reject empty pipeline
    if (pl->count < 2) {
        free_pipeline(pl);
        free(copy);
        return NULL;
    }


    free(copy);

    return pl;
}



void free_command(Command *cmd) {
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

void free_pipeline(Pipeline *pipeline)
{
    if (pipeline == NULL)
        return;

    for (int i = 0; i < pipeline->count; i++) {
        free_command(pipeline->commands[i]);
    }

    free(pipeline->commands);
    free(pipeline);
}

