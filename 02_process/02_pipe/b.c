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
    int fd = open("1.pipe", O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    Message m;
    ssize_t ret = read(fd, &m, sizeof(Message));
    ERROR_CHECK(ret, -1, "write");
    close(fd);
    //新建一个目录
    int d_ret = mkdir("dirForA", 0777);
    ERROR_CHECK(d_ret, -1, "mkdir");
    char file[4096] = { 0 };
    sprintf(file, "%s%s%s", "dirForA", "/", m.filename);
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    d_ret = ftruncate(fd, m.file_len);
    ERROR_CHECK(ret, -1, "ftruncate");
    ret = write(fd, m.content, sizeof(m.content));
    close(fd);
    return 0;
}