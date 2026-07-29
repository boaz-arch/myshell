#ifndef BUILTIN_H
#define BUILTIN_H


int is_builtin(const Command *cmd);

int execute_builtin(Command *cmd);

#endif

