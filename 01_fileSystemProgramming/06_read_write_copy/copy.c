#include <linux_c_stage.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    int src_fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(src_fd, -1, "open");
    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    ERROR_CHECK(dest_fd, -1, "open");
    ssize_t read_count;
    char buf[1024] = { 0 };
    while((read_count = read(src_fd, buf, sizeof(buf) - 1)))
    {
        write(dest_fd, buf, read_count);
    }
    close(src_fd);
    close(dest_fd);
    return 0;
}