#include <linux_c_stage.h>

int main(void)
{
    if(fork() == 0)
    {
        sleep(10);
        exit(NULL);
    }
    else {
        while(1);
    }
    return 0;
}