#include <stdio.h>

int main(void)
{
    int width = 10;
    char buf[1024]={0};
    printf("%*s\n", width, buf);
}