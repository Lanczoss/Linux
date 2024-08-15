#include <linux_c_stage.h>

int main(void)
{
    if(fork() == 0)
    {
        sleep(20);
    }
    else 
    {
        
    }
    return 0;
}