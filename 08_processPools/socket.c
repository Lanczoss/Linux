#include "head.h"


//创建socket对象
//第一个参数是返回的socket文件对象文件描述符
//第二个参数是ip，第三个是端口号
int init_socket(int *socket_fd, char *ip, char *port)
{
    *socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(*socket_fd == -1)
    {
        perror("socket");
        return -1;
    }
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(atoi(port));
    sockaddr.sin_addr.s_addr = inet_addr(ip);
    int ret = bind(*socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    if(ret == -1)
    {
        perror("bind");
        return -1;
    }
    ret = listen(*socket_fd, 10);
    if(ret == -1)
    {
        perror("listen");
        return -1;
    }
    return 0;
}
