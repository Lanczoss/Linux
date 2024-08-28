#include "head.h"

//自定义协议解决粘包问题
typedef struct train_s
{
    ssize_t size;
    char buf[1024];
}Train;

//子进程用于发送数据的函数
//参数需要对端发来的文件名
int sendFile(int net_fd, char *filename)
{
    //得到文件名
    //先发送文件大小，对端可以显示进度条
    Train train;
    memset(&train, 0, sizeof(train));
    int file_fd = open(filename, O_RDONLY);
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
    
    //发送文件
    while(1)
    {
        //每次读取文件一部分
        train.size = read(file_fd, train.buf, sizeof(train.buf));
        ERROR_CHECK(train.size, -1, "read");
        if(train.size == 0)
        {
            //发送完毕
            break;
        }
        printf("train size = %ld\n", train.size);
        //发送一部分，先发送长度
        ssize_t rret = send(net_fd, &train, sizeof(ssize_t) + train.size, MSG_NOSIGNAL);
        if(rret == -1)
        {
            break;
        }
    }
    close(file_fd);
    printf("send successfully to %d!\n", net_fd);
    return 0;
}

