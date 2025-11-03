#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Task {
    char description[200];
    int priority;
    struct Task *next;
} Task;

Task* add_task_to_end(Task *head, char *description, int priority);
Task* remove_task_by_name(Task *head, char *description);
void print_list_conditional(Task *head, int max_priority);
void print_list(Task *head);
void free_list(Task *head);

#endif