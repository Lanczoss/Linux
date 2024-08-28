#include "head.h"


//子进程用于发送数据的函数
//参数只需要net_fd
int sendFile(int net_fd)
{
    //发送hello
    int ret = send(net_fd, "hello", 5, 0);
    ERROR_CHECK(ret, -1, "send");
    printf("send successfully to %d!\n", net_fd);
    return 0;
}
