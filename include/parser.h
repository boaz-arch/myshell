#ifndef PARSER_H
#define PARSER_H

typedef struct {
    int argc;
    char **argv;
    
    char *input_file;
    
    char *output_file;
    int output_append;
        
    char * error_file;
    int error_append;

} Command;


typedef struct {
    Command *left;
    Command *right;
    
} Pipeline;


Command *parse_command(const char *input);
void free_command(Command *cmd);

Pipeline *parse_pipeline(const char *input);
void free_pipeline(Pipeline *pipeline);

#endif

