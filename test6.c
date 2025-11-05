#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "person.h"



int main(int argc, char *argv[]){
    if(argc!=3){
        printf("Usage: %s <filename> <count>\n", argv[0]);
        return 1;
    }
    char file_name[50];
    int persons_count;
    
    strcpy(file_name, argv[1]);
    persons_count = atoi(argv[2]);


Person *people_list = malloc(persons_count * sizeof(Person));
for(int i = 0; i<persons_count; i++){
    people_list[i].age = rand() % 100;
    strcpy(people_list[i].name, "john");
}

FILE *file = fopen(file_name, "w");
if (file == NULL){
    printf("error opening file for writing\n");
    return 1;
}

printf("writing people to file\n");
for(int i = 0; i<persons_count; i++){
    fprintf(file, "%s %d\n", people_list[i].name, people_list[i].age);
}
fclose(file);
free(people_list);

FILE *file_reading = fopen(file_name, "r");
if (file_reading == NULL){
    printf("error opening file for reading\n");
    return 1;
}

printf("reading people from file\n");
char name_buffer[50];
int age_buffer;
while(fscanf(file_reading, "%s %d\n", name_buffer, &age_buffer) == 2){
    Person p;
    strcpy(p.name, name_buffer);
    p.age = age_buffer;
    print_person(&p);
}
fclose(file_reading);
printf("done\n");

return 0;
}