#include <linux_c_stage.h>

/* Usage:  */
int main(void)
{
    int pipefd[2];
    int ret = pipe(pipefd);
    ERROR_CHECK(ret, -1, "pipe");
    if (fork() == 0)
    {
        close(pipefd[1]);
        while (1)
        {
            //验证写阻塞
            char buf[1024] = {0};
            ssize_t rw_ret = read(pipefd[0], buf, sizeof(buf));
            ERROR_CHECK(rw_ret, -1, "read");
            printf("read once\n");
        }
        close(pipefd[0]);
    }
    else
    {
        close(pipefd[0]);
        while (1)
        {
            //验证写阻塞
            char buf[4096] = {0};
            ssize_t rw_ret = write(pipefd[1], buf, sizeof(buf));
            ERROR_CHECK(rw_ret, -1, "write");
            printf("write once\n");
        }
        close(pipefd[1]);
    }
    return 0;
}
