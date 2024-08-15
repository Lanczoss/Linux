#include <linux_c_stage.h>

int main(void)
{
    int fd = open("1.pipe", O_WRONLY);
    ERROR_CHECK(fd, -1, "open");
    //行缓冲
    printf("Helloworld\n");
    int stdout_bak = 8;
    //备份
    int ret = dup2(STDOUT_FILENO, stdout_bak);
    ERROR_CHECK(ret, -1, "dup2");
    ret = dup2(fd, STDOUT_FILENO);
    ERROR_CHECK(ret, -1, "dup2");
    printf("Helloworld\n");
    //还原
    ret = dup2(stdout_bak, STDOUT_FILENO);
    ERROR_CHECK(ret, -1, "dup2");
    printf("Helloworld\n");
    close(fd);
    close(stdout_bak);
    return 0;
}