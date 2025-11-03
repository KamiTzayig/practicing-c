#include <stdio.h>
#include "task.h"


int main(){
    Task *head = NULL;
while (1){
    printf("options:\n1.list all tasks\n2.add a new task\n3.remove an existing task\n4.print tasks conditional\n5.quit\n");
    int option;
    scanf("%d", &option);
    if (option == 1){
        print_list(head);
    }
    else if (option == 2){
        char description[200];
        int priority;
        getchar();
        printf("what is the task description?\n");
        fgets(description, 200, stdin);
        description[strcspn(description, "\n")] = '\0';
        printf("what is the task priority?\n");
        scanf("%d", &priority);
        head = add_task_to_end(head, description, priority);
    }
    else if (option == 3){
        char description[200];
        printf("what is the task description?\n");
        getchar();
        fgets(description, 200, stdin);
        description[strcspn(description, "\n")] = '\0';
        head = remove_task_by_name(head, description);
    }

    else if (option == 4){
        int max_priority;
        printf("what is the maximum priority?\n");
        scanf("%d", &max_priority);
        print_list_conditional(head, max_priority);
    }
    else if (option == 5){
        break;
    }
    else{
        printf("invalid option\n");
    }
}
free_list(head);
return 0;
}