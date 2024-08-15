#include <linux_c_stage.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    close(STDIN_FILENO);
    int fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    int num;
    scanf("%d",&num);
    printf("%d\n", num);
    close(fd);
    return 0;
}