#include "shell.h"
#include "parser.h"
#include "executor.h"
#include "history.h"

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void print_prompt() {
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        printf("MyShell%s>> ", cwd);
    } else {
        printf("MyShell>> ");
    }
    
    fflush(stdout);
}

int read_input(char *input, size_t size) {
    if (fgets(input, size, stdin) == NULL)
        return 0;               

    input[strcspn(input, "\n")] = '\0';

    return 1;
}


void shell_loop(void) {
    char input[MAX_INPUT_SIZE];
    Command *cmd;

    while (1) {
        print_prompt();

        if (!read_input(input, sizeof(input))) {
            printf("\n");
            break;
        }

        if (input[0] == '\0')
            continue;
            
        history_add(input);

        cmd = parse_command(input);
        
        if (cmd == NULL) {
            fprintf(stderr, "parse failed\n");
            continue;
        }
        
        execute_command(cmd);
        free_command(cmd);
    }
}


int main() {
    history_init();
    shell_loop();
    history_cleanup();
    
    return 0;
}
