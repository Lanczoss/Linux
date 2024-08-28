#include "head.h"

//子进程用于发送数据的函数
//参数需要对端发来的文件名
int sendFile(int net_fd, char *filename)
{
    //得到文件名
    //先发送文件大小，对端可以显示进度条
    int file_fd = open(filename, O_RDWR);
    ERROR_CHECK(file_fd, -1, "open");
    //fstat可以快速获取文件大小
    struct stat st;
    memset(&st, 0, sizeof(st));
    int ret = fstat(file_fd, &st);
    ERROR_CHECK(ret, -1, "fstat");
    off_t file_size = st.st_size;
    printf("file_size = %ld\n", file_size);

    //MSG_NOSIGNAL当对方读端关闭时，不会让子进程崩溃
    ret = send(net_fd, &file_size, sizeof(off_t), MSG_NOSIGNAL);
    ERROR_CHECK(ret, -1, "send");

    //mmap零拷贝
    void *p = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_fd, 0);
    ERROR_CHECK(p, MAP_FAILED, "mmap");

    //sendfile函数直接发送至网卡
    ssize_t rret = sendfile(net_fd, file_fd, NULL, file_size);
    ERROR_CHECK(rret, -1, "sendfile");
    munmap(p, file_size);
    
    close(file_fd);
    printf("send successfully to %d!\n", net_fd);
    return 0;
}

