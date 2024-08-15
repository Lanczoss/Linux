#include <stdio.h>
void showRevertNum(int num){
     while (num >= 10){
        printf("%d", num % 10);
        num = num / 10;
     }
     printf("%d\n", num);
}

int main(void){
     int num;
     printf("Please input a number :");
     scanf("%d", &num);
     printf("After revert : ");
     showRevertNum(num);
}
