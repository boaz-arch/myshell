#ifndef PARSER_H
#define PARSER_H

typedef struct {
    int argc;
    char **argv;
    char *input_file;
    char *output_file;
    int append_mode;
} Command;


Command *parse_command(const char *input);

void free_command(Command *cmd);

#endif

