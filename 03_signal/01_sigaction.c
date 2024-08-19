#include <linux_c_stage.h>

void handler(int num)
{
    printf("I am 2\n");
}

int main(void)
{
    struct sigaction act, oldact;
    memset(&act, 0, sizeof(act));
    memset(&oldact, 0, sizeof(oldact));
    act.sa_flags = SA_RESTART;
    act.sa_handler = handler;

    //注册2号信号
    int ret = sigaction(SIGINT, &act, &oldact);
    ERROR_CHECK(ret, -1, "sigaction");

    char buf[1024] = {0};
    ssize_t r_ret = read(STDIN_FILENO, buf, sizeof(buf));
    ERROR_CHECK(r_ret, -1, "read");
    printf("%s\n", buf);

    return 0;
}

