#include <cstage.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    //创建socket对象
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socket_fd, -1, "socket");
    //建立连接
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(argv[1]);
    sockaddr.sin_port = htons(atoi(argv[2]));
    //设置ip和端口
    int ret = bind(socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    ERROR_CHECK(ret, -1, "bind");
    //开始监听
    int net_fd_arr[10];
    for(int i = 0; i < 10; i++)
    {
        net_fd_arr[i] = -1;
    }
    int net_fd;
    ret = listen(socket_fd, 10);
    ERROR_CHECK(ret, -1, "listen");
    //监听集合
    fd_set base_set;
    FD_ZERO(&base_set);
    FD_SET(socket_fd, &base_set);
    while(1)
    {
        fd_set set;
        memcpy(&set, &base_set, sizeof(base_set));
        ret = select(20, &set, NULL,  NULL, NULL);
        ERROR_CHECK(ret, -1, "select");
        if(FD_ISSET(socket_fd, &set))
        {
            //有全连接有新
            for(int i = 0; i < 10; i++)
            {
                if(net_fd_arr[i] == -1)
                {
                    net_fd = accept(socket_fd, NULL, NULL);
                    if(net_fd == -1)
                    {
                        printf("one client error with connecing\n");
                        continue;
                    }
                    net_fd_arr[i] = net_fd;
                    FD_SET(net_fd_arr[i], &base_set);
                    printf("connected one client, i = %d, [i] = %d!\n", i, net_fd_arr[i]);
                    break;
                }
            }
        }
        for(int i = 0; i < 10; i++)
        {
            if(FD_ISSET(net_fd_arr[i], &set))
            {
                char buf[60] = {0};
                ret = recv(net_fd_arr[i], buf, sizeof(buf), 0);
                ERROR_CHECK(ret, -1, "net_fd");
                if(ret == 0)
                {
                    net_fd_arr[i] = -1;
                    FD_CLR(net_fd_arr[i], &base_set);
                    close(net_fd_arr[i]);
                    printf("one client disconnected, i = %d, [i] = %d!\n", i, net_fd_arr[i]);
                    continue;
                }
                else
                {
                    for(int j = 0; j < 10; j++)
                    {
                        if(j == i || net_fd_arr[j] == -1)
                        {
                            continue;
                        }
                        ret = send(net_fd_arr[j], buf, sizeof(buf), 0);
                        ERROR_CHECK(ret, -1, "send");
                    }
                    printf("send success!\n");
                }
            }
        }
    }       
    close(socket_fd);
}
