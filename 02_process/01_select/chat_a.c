#include <linux_c_stage.h>

/* Usage:  */
int main(void)
{
    int fd_read = open("1.pipe", O_RDONLY);
    ERROR_CHECK(fd_read, -1, "open fd_read");
    int fd_write = open("2.pipe", O_WRONLY);
    ERROR_CHECK(fd_write, -1, "open fd_write");
    printf("聊天软件连接成功\n");
    //监听集合
    fd_set set;
    //计数器
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    while(1)
    {
        FD_ZERO(&set);
        FD_SET(fd_read, &set);
        FD_SET(STDIN_FILENO, &set);
        int ret = select(fd_read + 1, &set, NULL, NULL, &tv);
        ERROR_CHECK(ret, -1, "select");
        if(ret == 0)
        {
            //超时
            printf("超时，聊天软件退出中\n");
            break;
        }
        if(FD_ISSET(STDIN_FILENO, &set))
        {
            //标准输入就绪
            char buf[1024] = { 0 };
            ssize_t read_ret = read(STDIN_FILENO, buf, sizeof(buf));
            ERROR_CHECK(read_ret, -1, "read stdin");
            //如果读到0，说明标准输入EOF
            if(read_ret == 0)
            {
                printf("聊天软件已退出\n");
                break;
            }
            ssize_t write_ret = write(fd_write, buf, sizeof(buf));
            ERROR_CHECK(write_ret, -1, "write");
        }
        if(FD_ISSET(fd_read, &set))
        {
            //管道可读，打印
            char buf[1024] = { 0 };
            ssize_t read_ret = read(fd_read, buf, sizeof(buf));
            ERROR_CHECK(read_ret, -1, "read pipe");
            //如果读到0，说明对面发送了EOF
            if(read_ret == 0)
            {
                printf("对方已退出，聊天软件正在关闭\n");
                break;
            }
            printf("%s", buf);
            tv.tv_sec = 10;
            tv.tv_usec = 0;
        }
    }
    close(fd_read);
    close(fd_write);
    return 0;
}
