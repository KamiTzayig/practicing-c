#include <stdio.h>

void incriment(int *num){
    (*num) ++;
    return;
}
void decriment(int *num){
    (*num) --;
    return;
}



int main(){

int counter = 0;
char input;
printf("choose if to increment or decriment the counter, current value is %d\n", counter);
printf("press u to increment and d to decriment, any other key will close the program\n");


scanf(" %c", &input);
while(input != 'q'){
    if (input == 'u'){
        incriment(&counter);
    }
else if (input == 'd'){
    decriment(&counter);
}
else{
    printf("choose either u or d");
    }
printf("current value is %d\n", counter);

    scanf(" %c", &input);
};

printf("the final value is %d, bye bye!\n", counter);
return 0;

}
