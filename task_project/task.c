#include "task.h"
#include <string.h>


void print_list(Task *head){
    if(head == NULL){
        printf("List of tasks is empty\n");
        return;
    }

    for(Task *temp = head; temp != NULL; temp = temp->next){
        printf("#########################\ndescription: %s\npriority: %d\n######################################\n", temp->description, temp->priority);
    }
    return;
}

void print_list_conditional(Task *head, int max_priority){
    if(head == NULL){
        printf("List of tasks is empty\n");
        return;
    }

    for(Task *temp = head; temp != NULL; temp = temp->next){
        if(temp->priority <= max_priority){

            printf("#########################\ndescription: %s\npriority: %d\n######################################\n", temp->description, temp->priority);
        }
    }
    return;
}


void free_list(Task *head){
    if(head == NULL){
        return;
    }

    Task *temp = head;
    while(temp != NULL){
        head = head->next;
        free(temp);
        temp = head;
    }
    return;
}

Task* add_task_to_end(Task *head, char *description, int priority){
    Task *new_task = malloc(sizeof(Task));
    if (new_task == NULL){
        printf("Memory allocation failed for a new task\n");
        return head;
    }
    new_task->priority = priority;
    strcpy(new_task->description, description);
    new_task->next = NULL;

    if(head == NULL){
        return new_task;
    }
    Task *temp = head;
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = new_task;
    return head;
}



Task* remove_task_by_name(Task *head, char *description){
    if(head == NULL){
        return NULL;
    }

    if (strcmp(head->description, description) == 0){
        Task *new_head = head->next;
        free(head);
        return new_head;
    }

    Task *prev = NULL;
    Task *current = head;

    while(current != NULL && strcmp(current->description, description) != 0){
        prev = current;
        current = current->next;
    }

    if(current == NULL){
        return head;
    }

    prev->next = current->next;
    free(current);
    return head;

}