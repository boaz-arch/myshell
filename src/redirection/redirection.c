#include "redirection.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int apply_redirections(Command *cmd) {
    if (cmd == NULL)
        return -1;
    
    
    // Input redirection: <
    if (cmd->input_file != NULL) {

        int fd = open(cmd->input_file, O_RDONLY);

        if (fd < 0) {
            perror("open");
            return -1;
        }

        if (dup2(fd, STDIN_FILENO) < 0) {
            perror("dup2");
            close(fd);
            return -1;
        }

        close(fd);
    }


    // Output redirection: > or >>
    if (cmd->output_file != NULL){
        int flags = O_WRONLY | O_CREAT;
        
        if (cmd->output_append){
            flags |= O_APPEND;
        } else {
            flags |= O_TRUNC;
        }
        
        int fd = open(cmd->output_file, flags, 0644);
        
        if (fd < 0){
            perror("open");
            return -1;
        }
        
        if (dup2(fd, STDOUT_FILENO) < 0){
            perror("dup2");
            close(fd);
            return -1;
        }
        
        close(fd);
    
    }


    return 0;
}

