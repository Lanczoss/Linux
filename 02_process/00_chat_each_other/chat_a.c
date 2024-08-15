#include <linux_c_stage.h>

int main(void)
{
    int fd_read = open("1.pipe", O_RDONLY);
    ERROR_CHECK(fd_read, -1, "open fd_read");
    int fd_write = open("2.pipe", O_WRONLY);
    ERROR_CHECK(fd_write, -1, "open fd_write");
    //buffer
    char buf[1024] = { 0 };
    //listen set
    fd_set set;
    printf("聊天软件连接成功\n");
    while(1)
    {
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        FD_SET(fd_read, &set);
        int i_ret = select(fd_write + 1, &set, NULL, NULL, NULL);
        ERROR_CHECK(i_ret, -1, "select");
        if(FD_ISSET(STDIN_FILENO, &set))
        {
            //stdin ready
            bzero(buf, sizeof(buf));
            //time
            time_t t = time(NULL);
            struct tm *lt = localtime(&t);
            ssize_t ret = write(fd_write, lt, sizeof(struct tm));
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read stdin");
            if(ret == 0)
            {
                //stdin EOF
                printf("聊天软件正在关闭\n");
                break;
            }
            ret = write(fd_write, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "write stdin");
        }
        if(FD_ISSET(fd_read, &set))
        {
            //pipe read ready
            bzero(buf, sizeof(buf));
            struct tm aross_lt;
            ssize_t ret = read(fd_read, &aross_lt, sizeof(struct tm));
            ERROR_CHECK(ret, -1, "read time");
            if(ret == 0)
            {
                //write close
                printf("对方已退出，聊天软件正在关闭\n");
                break;
            }
            ret = read(fd_read, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read pipe");
            if(ret == 0)
            {
                //write close
                printf("对方已退出，聊天软件正在关闭\n");
                break;
            }
            printf("%02d-%02d %02d:%02d\n", aross_lt.tm_mon + 1, aross_lt.tm_mday, aross_lt.tm_hour, aross_lt.tm_min);
            printf("%s\n", buf);
        }
    }
    close(fd_read);
    close(fd_write);
    return 0;
}
