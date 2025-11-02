#include "person.h"
#include <stdio.h>

void print_person(struct Person *p){
    printf("hello %s, you are %d years old\n", p->name, p->age);
}