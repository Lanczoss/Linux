#include <linux_c_stage.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    printf("This is first message.\n");
    close(STDOUT_FILENO);
    int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
    ERROR_CHECK(fd, -1, "open");
    printf("This is second message.\n");
    //fflush(stdout);
    close(fd);
    return 0;
}