#ifndef HEAD_H
#define HEAD_H

#include <cstage.h>
#include "queue.h"

//维护一个共有资源
typedef struct status_s
{
    //子线程记录所有子线程id
    pthread_t *id_list;
    //子线程个数
    int num;
    //存储net_fd的队列
    Queue q;
    //互斥锁
    pthread_mutex_t lock;
    //条件变量
    pthread_cond_t cond;
    //退出标志
    int exit_flag;
}Status;

//初始化子线程
//第一个参数是共有资源，第二个参数是线程个数
int initThreads(Status *status, int thread_num);

//初始化socket对象
//第一个参数是socket对象的fd
//第二个参数是ip地址
//第三个参数是端口号
int initSocket(int *socket_fd, char *ip, char *port);

//将某fd加入epoll监听集合
//第一个参数是epoll对象的fd
//第二个参数是需要加入到监听集合的fd
int addEpoll(int epoll_fd, int fd);

//子线程主函数
//参数是从主线程接收的参数
void *threadMain(void *p);

//子线程工作函数
//参数传递的是子线程用的net_fd
int doWorker(int net_fd);

//用于发送数据的函数
//参数需要对端发来的文件名
int sendFile(int net_fd, char *filename);

//发送文件的总逻辑
//参数是传输用的net_fd
int toClientFile(int net_fd);

//用于接收文件名的函数
//第一个参数是net_fd
//第二个参数是存文件名的空间
int recvFileName(int net_fd, char *filename);

//用于判断是否有这个文件名
//第一个参数是传输用net_fd
//第二个参数是收到的文件名
int doesHaveFile(int net_fd, char *filename);

#endif
