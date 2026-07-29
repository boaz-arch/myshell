#include "history.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static History history;

void history_init(void)
{
    history.size = 0;
    history.capacity = 10;

    history.entries = malloc(history.capacity * sizeof(char *));
    if (history.entries == NULL)
        history.capacity = 0;
}

void history_add(const char *command)
{
    if (history.entries == NULL)
        return;

    if (history.size == history.capacity) {
        int new_capacity = history.capacity * 2;

        char **new_entries = realloc(
            history.entries,
            new_capacity * sizeof(char *)
        );

        if (new_entries == NULL)
            return;

        history.entries = new_entries;
        history.capacity = new_capacity;
    }

    history.entries[history.size] = strdup(command);
    if (history.entries[history.size] == NULL)
        return;

    history.size++;
}

void history_print(void)
{
    for (int i = 0; i < history.size; i++)
        printf("%d %s\n", i + 1, history.entries[i]);
}

void history_cleanup(void)
{
    for (int i = 0; i < history.size; i++)
        free(history.entries[i]);

    free(history.entries);

    history.entries = NULL;
    history.size = 0;
    history.capacity = 0;
}



