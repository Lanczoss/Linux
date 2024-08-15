#include <linux_c_stage.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    printf("This is first message.\n");
    int stdout_bak = 8;
    dup2(STDOUT_FILENO, stdout_bak);
    // stdout重定向至文件
    int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
    ERROR_CHECK(fd, -1, "open");
    dup2(fd, STDOUT_FILENO);
    printf("This is sencond message for stdout.\n");
    //复原
    dup2(stdout_bak, STDOUT_FILENO);
    printf("This is third message for stdout.\n");
    close(fd);
    close(stdout_bak);
    return 0;
}