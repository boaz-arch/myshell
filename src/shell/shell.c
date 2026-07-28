#include <shell.h>
#include <parser.h>
#include <executor.h>

#include<stdio.h>
#include <stdlib.h>
#include <string.h>

void print_prompt() {
    printf("MyShell>> ");
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

    while (1) {
        print_prompt();

        if (!read_input(input, sizeof(input))) {
            printf("\n");
            break;
        }

        if (input[0] == '\0')
            continue;

        parse_input(input);
        execute_command();
        free_mem();
    }
}


int main() {
    shell_loop();
    
    return 0;
}
