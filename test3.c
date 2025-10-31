#include <stdio.h>

int sum_array(int arr[], int size){
    int sum = 0;
    for (int i = 0; i<size; i++){
        sum += arr[i];
    }
    return sum;
}


int main(){

    int score[5] = {};
  int score_sum = 0;
    
    printf("enter 5 numbers: \n");
    for (int i = 0; i<5; i++){
        scanf("%d", &score[i]);
    }
    
    printf("your total score is: \n");

  score_sum = sum_array(score, 5);
    printf("%d\n", score_sum);
    
    return 0;


}