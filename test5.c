#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct Person{
    char name[50];
    int age;
};
void print_person(struct Person *p){
        printf("hello %s, you are %d years old\n", p->name, p->age);
}
int main(){
struct Person p1;
int persons_count = 0;
printf("how many people do you want to create?\n");
scanf("%d", &persons_count);

struct Person *people_list = malloc(persons_count * sizeof(struct Person));
for(int i = 0; i<persons_count; i++){
    people_list[i].age = rand() % 100;
    strcpy(people_list[i].name, "john");
}

FILE *file = fopen("people.txt", "w");
if (file == NULL){
    printf("error opening file\n");
    return 1;
}
for(int i = 0; i<persons_count; i++){
    print_person(&people_list[i]);
    printf("writing to file\n");
    fprintf(file, "%s %d\n", people_list[i].name, people_list[i].age);
}
fclose(file);
free(people_list);
return 0;
}