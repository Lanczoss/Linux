#include <linux_c_stage.h>

int main(void)
{
    int fd = open("1.pipe", O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    fd_set set_w, set_r;
    while (1)
    {
        FD_ZERO(&set_w);
        FD_ZERO(&set_r);
        FD_SET(fd, &set_w);
        FD_SET(fd, &set_r);
        int ret = select(fd + 1, &set_r, &set_w, NULL, NULL);
        ERROR_CHECK(ret, -1, "select");
        if (FD_ISSET(fd, &set_w))
        {
            //写就绪,一次写4096
            char buf[4096] = {0};
            ssize_t s_ret = write(fd, buf, sizeof(buf));
            ERROR_CHECK(s_ret, -1, "write");
            printf("write once\n");
        }
        if (FD_ISSET(fd, &set_r))
        {
            //读就绪，一次读1024
            char buf[1024] = {0};
            ssize_t s_ret = read(fd, buf, sizeof(buf));
            ERROR_CHECK(s_ret, -1, "read");
            printf("read once\n");
        }
    }
    close(fd);
    return 0;
}