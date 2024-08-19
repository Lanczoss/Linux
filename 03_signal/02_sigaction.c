#include <linux_c_stage.h>

void handler(int num)
{
    printf("I am 2\n");
    sleep(5);
    printf("2 is leave\n");
}

int main(void)
{
    struct sigaction act, oldact;
    memset(&act, 0, sizeof(act));
    memset(&oldact, 0, sizeof(oldact));
    sigset_t set;
    sigemptyset(&set);
    //临时屏蔽3号信号
    sigaddset(&set, 3);
    act.sa_mask = set;
    act.sa_handler = handler;

    //注册2号信号
    int ret = sigaction(SIGINT, &act, &oldact);
    ERROR_CHECK(ret, -1, "sigaction");
    sleep(5);

    return 0;
}

