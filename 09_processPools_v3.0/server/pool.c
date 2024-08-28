#include "head.h"


//创建并初始化子进程
//第一个参数为子进程状态数组，第二个参数为数组长度
int init_pool(son_status_t *son_list, int num)
{
    //一共创建num个进程
    for(int i = 0; i < num; i++)
    {
        //创建本地socket
        int socket_fd[2];
        int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, socket_fd);
        ERROR_CHECK(ret, -1, "socketpair");
        pid_t pid = fork();
        if(pid == 0)
        {
            //脱离前台进程组
            setpgid(0, 0);
            //son使用socket_fd[0]通信
            close(socket_fd[1]);
            ret = do_worker(socket_fd[0]);
            ERROR_CHECK(ret, -1, "do_worker");
        }
        else
        {
            //parent使用socket_fd[1]通信
            close(socket_fd[0]);
            //记录子进程信息
            son_list[i].pid = pid;
            son_list[i].local_socket = socket_fd[1];
            son_list[i].son_status = FREE;
            printf("No.%d process created, socket_fd = %d!\n", i, socket_fd[1]);
        }
    }
    return 0;
}


//将net_fd通过本地socket发送到子进程中
int net_fd_to_son(son_status_t *son_list, int num, int net_fd)
{
    for(int i = 0; i < num; i++)
    {
        if(son_list[i].son_status == FREE)
        {
            int ret = sendMsg(son_list[i].local_socket, net_fd, 0);
            ERROR_CHECK(ret, -1, "sendMsg");
            son_list[i].son_status = BUSY;
            printf("sendMsg net_fd = %d success!\n", net_fd);
            break;
        }
    }
    return 0;
}
