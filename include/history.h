#ifndef HISTORY_H
#define HISTORY_H

typedef struct {
    char **entries;    
    int size;
    int capacity;
} History;

void history_init(void);

void history_add(const char *command);

void history_print(void);

void history_cleanup(void);

#endif

