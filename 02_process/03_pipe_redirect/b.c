#include <linux_c_stage.h>

int main(void)
{
    int fd = open("1.pipe", O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    char buf[1024] = { 0 };
    ssize_t ret = read(fd, buf, sizeof(buf));
    ERROR_CHECK(ret, -1, "read");
    printf("%s\n", buf);
    close(fd);
    return 0;
}