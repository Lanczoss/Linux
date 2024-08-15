#include <stdio.h>

void print_mutiplication(int len)
{
    for (int i = 1; i <= len; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            printf("%d x %d = %-2d ", j, i, i * j);
        }
        printf("\n");
    }
}

int main()
{
    int len;
    printf("Please enter the number to print mutiplication:\n");
    scanf("%d", &len);
    print_mutiplication(len);
    return 0;
}
