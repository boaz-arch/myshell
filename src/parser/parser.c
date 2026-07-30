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
        
        else if (strcmp(token, ">>") == 0){
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
        
        else if (strcmp(token, "<") == 0){
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
    const char *pipe = strchr(input, '|');

    if (pipe == NULL)
        return NULL;

    Pipeline *pl = malloc(sizeof(Pipeline));
    if (pl == NULL)
        return NULL;

    size_t left_len = pipe - input;

    char *left = malloc(left_len + 1);
    if (left == NULL) {
        free(pl);
        return NULL;
    }

    memcpy(left, input, left_len);
    left[left_len] = '\0';

    char *right = strdup(pipe + 1);
    if (right == NULL) {
        free(left);
        free(pl);
        return NULL;
    }
    
    pl->left = parse_command(left);
    pl->right = parse_command(right);

    free(left);
    free(right);

    if (pl->left == NULL || pl->right == NULL) {
        free_command(pl->left);
        free_command(pl->right);
        free(pl);
        return NULL;
    }
    
    return pl;
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


void free_pipeline(Pipeline *pipeline)
{
    if (pipeline == NULL)
        return;

    free_command(pipeline->left);
    free_command(pipeline->right);

    free(pipeline);
}

