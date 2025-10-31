#include <stdio.h>



int main(){
float side_a, side_b, area= -1;
printf("enter the first side of the rectangle:");
scanf("%f", &side_a);
printf("enter the second side of the rectangle:");
scanf("%f", &side_b);
area = side_a * side_b;
printf("the area of the rectangle is %f\n", area);
if(area>100){
    printf("thats a BIG rectangle!\n");
}
return 0;
}
