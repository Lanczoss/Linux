#ifndef HEAD_H
#define HEAD_H

#include <cstage.h>

//枚举子进程状态
enum
{
    BUSY,
    FREE
};
//父进程用于记录子进程的相关信息
typedef struct
{
    //子进程pid
    pid_t pid;
    //接收子进程消息用的本地socket
    int local_socket;
    int son_status;
}son_status_t;

//创建并初始化子进程
//第一个参数为子进程状态数组，第二个参数为数组长度
int init_pool(son_status_t *son_list, int num);

//创建socket对象
//第一个参数是返回的socket文件对象文件描述符
//第二个参数是ip，第三个是端口号
int init_socket(int *socket_fd, char *ip, char *port);

//父进程将需要监听的文件描述符加入到epoll监听集合
//第一个参数是epoll的文件描述符
//第二个参数是需要添加的文件描述符
int add_epoll(int epoll_fd, int fd);

//将net_fd通过本地socket发送到子进程中
int net_fd_to_son(son_status_t *son_list, int num, int net_fd);

//子进程核心函数
//参数传递的是子进程用的本地socket
int do_worker(int socket_fd);

//通过本地socket发送文件描述符
//第一个参数是通信用本地socket
//第二个参数是要发送的文件描述符
int sendMsg(int local_socket, int net_fd);

//通过本地socket接收文件描述符
//第一个参数是通信用本地socket
//第二个传入传出参数是接收的netfd
int recvMsg(int local_socket, int *net_fd);

//子进程用于发送数据的函数
//参数需要对端发来的文件名
int sendFile(int net_fd, char *filename);

//子进程发送文件的总逻辑
//参数是传输用的net_fd
int toClientFile(int net_fd);

#endif
