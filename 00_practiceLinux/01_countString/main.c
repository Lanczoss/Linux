#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRING_LEN 1024

void count_string(char *str)
{
    int letter = 0;
    int number = 0;
    int blank_space = 0;
    char *curr = str;
    while(*curr != '\0')
    {
        if((*curr >= 'A' && *curr <= 'Z') || (*curr >= 'a' && *curr <= 'z'))
        {
            letter++;
        }
        if(*curr >= '0' && *curr <= '9')
        {
            number++;
        }
        if(*curr == ' ')
        {
            blank_space++;
        }
        curr++;
    }
    printf("There are %d letters, %d numbers and %d blankspaces in the string.\n",
           letter, number, blank_space);
}

int main()
{
    char str[STRING_LEN] = { 0 };
    scanf("%[^\n]s",str);
    count_string(str);
    return 0;
}

