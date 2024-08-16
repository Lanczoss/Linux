#include <linux_c_stage.h>

void func(int num)
{
    printf("%d accept\n", num);
    sleep(10);
    printf("%d go away\n", num);
}

int main(void)
{
    __sighandler_t ret = signal(2, func);
    ERROR_CHECK(ret, SIG_ERR, "signal");
    sleep(2);
    return 0;
}