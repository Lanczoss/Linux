#include <linux_c_stage.h>

int main(void)
{
    sigset_t act, oldact, check;
    int ret = sigemptyset(&act);
    ERROR_CHECK(ret, -1, "sigeemptyset act");
    ret = sigemptyset(&oldact);
    ERROR_CHECK(ret, -1, "sigemptyset oldact");
    ret = sigemptyset(&check);
    ERROR_CHECK(ret, -1, "sigemptyset check");
    //屏蔽2号信号
    ret = sigaddset(&act, 2);
    ERROR_CHECK(ret, -1, "sigaddset 2");
    //全程屏蔽
    ret = sigprocmask(SIG_BLOCK, &act, &oldact);
    ERROR_CHECK(ret, -1, "sigprocmask block");
    sleep(5);
    ret = sigpending(&check);
    ERROR_CHECK(ret, -1, "sigpending");
    if(sigismember(&check, 2))
    {
        printf("有信号被挂起\n");
    }
    //解除屏蔽
    ret = sigprocmask(SIG_UNBLOCK, &act, &oldact);
    ERROR_CHECK(ret, -1, "sigprocmask unblock");
    return 0;
}

