#include "head.h"

//自己通知自己退出
int pipefd[2];

void func(int num)
{
    printf("got 2\n");
    ssize_t rret = write(pipefd[1], "exit", 4);
    if(rret == -1)
    {
        printf("write pipe error.\n");
        exit(-1);
    }
}

int main(int argc, char *argv[])
{
    //创建匿名管道，注册2号信号
    int ret = pipe(pipefd);
    ERROR_CHECK(ret, -1, "pipe");
    if(fork() != 0)
    {
        close(pipefd[0]);
        //父进程专门接收信号
        signal(SIGINT, func);
        wait(NULL);
        //子进程已经关闭
        printf("son quit! parent quit now.\n");
        exit(0);
    }
    close(pipefd[1]);
    setpgid(0, 0);
    ARGS_CHECK(argc, 3);
    //一定先初始化锁和条件变量
    Status status;
    memset(&status, 0, sizeof(Status));
    ret = pthread_mutex_init(&status.lock, NULL);
    ERROR_CHECK(ret, -1, "lock init");
    ret = pthread_cond_init(&status.cond, NULL);
    ERROR_CHECK(ret, -1, "cond init");
    status.exit_flag = 0;
    status.num = 8;
    //获取socket对象的fd
    int socket_fd;
    ret = initSocket(&socket_fd, argv[1], argv[2]);
    ERROR_CHECK(ret, -1, "initSokcet");
    //epoll对象
    int epoll_fd = epoll_create(1);
    ERROR_CHECK(epoll_fd, -1, "epoll_create");
    //socket_fd加入监听
    ret = addEpoll(epoll_fd, socket_fd);
    ERROR_CHECK(ret, -1, "addEpoll");
    //管道读加入监听
    ret = addEpoll(epoll_fd, pipefd[0]);
    ERROR_CHECK(ret, -1, "addEpoll");
    //初始化子线程
    ret = initThreads(&status, status.num);
    ERROR_CHECK(ret, -1, "initThreads");

    //while循环开始监听
    while(1)
    {
        struct epoll_event event[10];
        int epoll_num = epoll_wait(epoll_fd, event, 10, -1);
        ERROR_CHECK(epoll_num, -1, "epoll_wait");
        for(int i = 0; i < epoll_num; i++)
        {
            int fd = event[i].data.fd;
            if(fd == pipefd[0])
            {
                //准备退出
                char buf[60] = {0};
                ssize_t rret = read(pipefd[0], buf, sizeof(buf));
                ERROR_CHECK(rret, -1, "read pipe");
                ret = pthread_mutex_lock(&status.lock);
                ERROR_CHECK(ret, -1, "lock");
                for(int j = 0; j < status.num; j++)
                {
                    //使用pthread cancel被动退出
                    ret = pthread_cancel(status.id_list[j]);
                    ERROR_CHECK(ret, -1, "pthread_cancel");
                }
                ret = pthread_mutex_unlock(&status.lock);
                ERROR_CHECK(ret, -1, "unlock");
                printf("wait threads.\n");
                for(int j = 0; j < status.num; j++)
                {
                    ret = pthread_join(status.id_list[j], NULL);
                    ERROR_CHECK(ret, -1, "join");
                    printf("No.%d thread quit.\n", j);
                }
                free(status.id_list);
                //清理队列
                while(status.q.head != NULL)
                {
                    int net_fd;
                    deQueue(&status.q, &net_fd);
                }
                close(socket_fd);
                close(epoll_fd);
                pthread_exit(NULL);
            }
            if(fd == socket_fd)
            {
                //有新连接
                int net_fd = accept(socket_fd, NULL, NULL);
                ERROR_CHECK(net_fd, -1, "accept");
                //将net_fd添加进队列
                ret = pthread_mutex_lock(&status.lock);
                THREAD_ERROR_CHECK(ret, "lock");
                ret = enQueue(&status.q, net_fd);
                THREAD_ERROR_CHECK(ret, "enQueue");
                ret = pthread_cond_broadcast(&status.cond);
                THREAD_ERROR_CHECK(ret, "broadcast");
                ret = pthread_mutex_unlock(&status.lock);
                THREAD_ERROR_CHECK(ret, "broadcast");
                printf("one client arrive.\n");
            }
        }
    }
    close(socket_fd);
    close(epoll_fd);
    return 0;
}
