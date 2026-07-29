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


