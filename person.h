#ifndef PERSON_H
#define PERSON_H

typedef struct Person {
    char name[50];
    int age;
} Person;

void print_person(Person *p);

#endif