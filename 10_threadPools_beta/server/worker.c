#include "head.h"

//子线程核心函数
//参数传递是子线程接收的net_fd
int doWorker(int net_fd)
{
    //读取任务
    printf("recv net_fd = %d success!\n", net_fd);
    //工作
    int ret = toClientFile(net_fd);
    ERROR_CHECK(ret, -1, "sendFile");
    //关闭接收的文件描述符
    close(net_fd);
    return 0;
}


//子进程发送文件的总逻辑
//参数是传输用的net_fd
int toClientFile(int net_fd)
{
    //接收文件名
    char filename[60] = {0};
    int ret = recvFileName(net_fd, filename);
    ERROR_CHECK(ret, -1, "recvFileName");
    //判断文件是否存在
    ret = doesHaveFile(net_fd, filename);
    if(ret == -1)
    {
        //说明文件不存在
        printf("client %d need %s doesn't exist.\n", net_fd, filename);
        return 0;
    }
    //发送数据
    ret = sendFile(net_fd, filename);
    ERROR_CHECK(ret, -1, "sendFile");
    return 0;
}
