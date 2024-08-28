#include "head.h"


//子进程核心函数
//参数传递的是子进程用的本地socket
int do_worker(int socket_fd)
{
    while(1)
    {
        //读取任务
        int net_fd;
        int ret = recvMsg(&net_fd);
        if(ret == -1)
        {
            perror("recvMsg");
            return -1;
        }
        //工作

        //通知父进程修改状态
        ssize_t rret = send(socket_fd, "IamFREE", 7, 0);
        if(rret == -1)
        {
            perror("send");
            return -1;
        }
    }
    return 0;
}
