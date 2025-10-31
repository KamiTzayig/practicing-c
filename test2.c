#include <stdio.h>

int sum_to_n(int n){
    int sum = 0;
    for (int i = 0; i<=n; i++){
        sum += i;
    }
    return sum;
}

int main(){

    int num = -1;
    int sum = 0;

    scanf("%d", &num);

    if (num < 1) {
        printf("you picked %d, pick a number that is bigger than 0\n", num);
        scanf("%d", &num);
    }
   
 sum = sum_to_n(num);
   
   
    printf("the sum is %d\n", sum);
    return 0;
}