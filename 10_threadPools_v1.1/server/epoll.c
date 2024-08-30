#include "head.h"

//将某fd加入epoll监听集合
//第一个参数是epoll对象的fd
//第二个参数是需要加入到监听集合的fd
int addEpoll(int epoll_fd, int fd)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    printf("addEpoll %d success.\n", fd);
    return 0;
}
