#include <linux_c_stage.h>
#define FILENAME 1024
#define CONTENT 1024

typedef struct
{
    char filename[FILENAME];
    int file_len;
    char content[CONTENT];
}Message;

int main(void)
{
    //进程A负责发送文件名字，长度和内容
    Message m = {"File", 6, "12345"};
    int fd = open("1.pipe", O_WRONLY);
    ERROR_CHECK(fd, -1, "open");
    ssize_t ret = write(fd, &m, sizeof(Message));
    ERROR_CHECK(ret, -1, "write");
    close(fd);
    return 0;
}