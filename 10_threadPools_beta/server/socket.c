#include "head.h"


//初始化socket对象
//第一个参数是socket对象的fd
//第二个参数是ip地址
//第三个参数是端口号
int initSocket(int *socket_fd, char *ip, char *port)
{
    *socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(*socket_fd, -1, "socket");
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(atoi(port));
    sockaddr.sin_addr.s_addr = inet_addr(ip);
    int ret = bind(*socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(*socket_fd, 10);
    return 0;
}
