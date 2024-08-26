#include <cstage.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socket_fd, -1, "socket");
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(atoi(argv[2]));
    sockaddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = connect(socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    ERROR_CHECK(ret, -1, "connect");
    //epoll_create
    int epoll_fd = epoll_create(1);
    ERROR_CHECK(epoll_fd, -1, "epoll_create");
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = socket_fd;
    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    event.data.fd = STDIN_FILENO;
    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    int flag = 0;
    while(1)
    {
        struct epoll_event list[10];
        int epoll_num = epoll_wait(epoll_fd, list, 10, -1);
        ERROR_CHECK(epoll_num, -1, "select");
        for(int i = 0; i < epoll_num; i++)
        {
            int fd = list[i].data.fd;
            if(fd == STDIN_FILENO)
            {
                char buf[60] = {0};
                ssize_t rret = read(STDIN_FILENO, buf, sizeof(buf) - 1);
                ERROR_CHECK(rret, -1, "read");
                //EOF
                if(rret == 0)
                {
                    printf("disconnected!\n");
                    break;
                }
                rret = send(socket_fd, buf, sizeof(buf), 0);
                ERROR_CHECK(rret, -1, "send");
            }
            if(fd == socket_fd)
            {
                char buf[60] = {0};
                ssize_t rret = recv(socket_fd, buf, sizeof(buf), 0);
                ERROR_CHECK(rret, -1, "recv");
                if(rret == 0)
                {
                    //server close
                    printf("server quit!\n");
                    flag = 1;
                    break;
                }
                printf("%s\n", buf);
            }
        }
        if(flag == 1)
        {
            break;
        }
    }
    close(socket_fd);
}
