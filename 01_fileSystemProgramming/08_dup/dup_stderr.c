#include <linux_c_stage.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 1);
    fprintf(stderr, "This is first message.\n");
    close(STDERR_FILENO);
    int fd = open("error.log", O_RDWR | O_CREAT | O_TRUNC, 0666);
    ERROR_CHECK(fd, -1, "open");
    fprintf(stderr, "This is second message.\n");
    close(fd);
    return 0;
}