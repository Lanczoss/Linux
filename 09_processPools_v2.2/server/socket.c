#include "head.h"


//创建socket对象
//第一个参数是返回的socket文件对象文件描述符
//第二个参数是ip，第三个是端口号
int init_socket(int *socket_fd, char *ip, char *port)
{
    *socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(*socket_fd, -1, "socket");
    int reuse = 1;
    setsockopt(*socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(atoi(port));
    sockaddr.sin_addr.s_addr = inet_addr(ip);
    int ret = bind(*socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(*socket_fd, 10);
    ERROR_CHECK(ret, -1, "listen");
    return 0;
}
