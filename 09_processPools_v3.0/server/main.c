#include "head.h"

//创建匿名管道
int pipefd[2];
void func(int num)
{
    printf("got 2\n");
    //父进程接收2号信号的处理函数
    //向pipefd[1]发送数据
    ssize_t rret = write(pipefd[1], "Quit!", 5);
    if(rret == -1)
    {
        perror("write");
        exit(-1);
    }
}

int main(int argc, char *argv[])
{
    //创建八个子进程（8核）
    son_status_t son_list[8];
    int ret = init_pool(son_list, 8);
    ERROR_CHECK(ret, -1, "init_pool");
    //创建socket对象并监听
    int socket_fd;
    ret = init_socket(&socket_fd, argv[1], argv[2]);
    ERROR_CHECK(ret, -1, "init_socket");
    //创建epoll对象
    int epoll_fd = epoll_create(1);
    ERROR_CHECK(epoll_fd, -1, "epoll_create");
    //监听socket和子进程状态
    ret = add_epoll(epoll_fd, socket_fd);
    ERROR_CHECK(ret, -1, "add_epoll");
    for(int i = 0; i < 8; i++)
    {
        ret = add_epoll(epoll_fd, son_list[i].local_socket);
        ERROR_CHECK(ret, -1, "add_epoll");
    }
    //注册2号信号
    //匿名管道用于信号处理函数通知父进程该退出了
    ret = pipe(pipefd);
    ERROR_CHECK(ret, -1, "pipe");
    signal(2, func);
    //pipefd[0]用于读，加入监听
    ret = add_epoll(epoll_fd, pipefd[0]);
    ERROR_CHECK(ret, -1, "add_epoll");
    //while循环中找到空闲子进程，传送文件描述符
    //或者接收子进程状态，将子进程状态改为空闲
    while(1)
    {
        struct epoll_event event_list[10];
        memset(event_list, 0, sizeof(event_list));
        //epoll wait等待就绪
        int epoll_num = epoll_wait(epoll_fd, event_list, 10, -1);
        for(int i = 0; i < epoll_num; i++)
        {
            int fd = event_list[i].data.fd; 
            if(fd == pipefd[0])
            {
                printf("pipefd wake\n");
                //如果是信号处理函数发起通知
                char buf[60] = {0};
                ssize_t rret = read(pipefd[0], buf, sizeof(buf));
                ERROR_CHECK(rret, -1, "read pipefd");
                //修改所有子进程接收的flag
                for(int j = 0; j < 8; j++)
                {
                    //随意发送一个net_fd
                    ret = sendMsg(son_list[j].local_socket, 0, -1);
                    ERROR_CHECK(ret, -1, "sendMsg");
                }
                for(int j = 0; j < 8; j++)
                {
                    wait(NULL);
                    printf("No.%d process quit!\n", j);
                }
                close(socket_fd);
                close(epoll_fd);
                close(pipefd[0]);
                close(pipefd[1]);
                printf("Clean complete! Main now quit.\n");
                exit(0);
            }
            //如果有新链接到来
            if(fd == socket_fd)
            {
                int net_fd = accept(socket_fd, NULL, NULL);
                ERROR_CHECK(net_fd, -1, "accpet");
                //交给子进程
                printf("one client connected!\n");
                ret = net_fd_to_son(son_list, 8, net_fd);
                //net_fd已经用socketpair传输，可在父进程这边关闭
                close(net_fd);
                continue;
            }
            //从这里开始说明是子进程数组里发生状态变化
            //找到是哪一个子进程
            for(int j = 0; j < 8; j++)
            {
                if(fd == son_list[j].local_socket)
                {
                    //将子进程记录为空闲状态
                    son_list[j].son_status = FREE;
                    //本次循环只有一个fd
                    break;
                }
            }
        }
    }
    close(socket_fd);
    close(epoll_fd);
}
