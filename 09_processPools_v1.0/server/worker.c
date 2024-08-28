#include "head.h"

//子进程核心函数
//参数传递的是子进程用的本地socket
int do_worker(int socket_fd)
{
    while(1)
    {
        printf("I am in\n");
        //读取任务
        int net_fd;
        int ret = recvMsg(socket_fd, &net_fd);
        ERROR_CHECK(ret, -1, "recvMsg");
        printf("recv net_fd = %d success!\n", net_fd);
        //工作
        ret = sendFile(net_fd);
        ERROR_CHECK(ret, -1, "sendFile");
        //通知父进程修改状态
        ssize_t rret = send(socket_fd, "IamFREE", 7, 0);
        ERROR_CHECK(rret, -1, "send");
    }
    return 0;
}
