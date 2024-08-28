#include "head.h"


//父进程将需要监听的文件描述符加入到epoll监听集合
//第一个参数是epoll的文件描述符
//第二个参数是需要添加的文件描述符
int add_epoll(int epoll_fd, int fd)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    return 0;
}
