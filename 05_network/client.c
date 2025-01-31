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
    printf("connected!\n");
    fd_set set;
    while(1)
    {
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        FD_SET(socket_fd, &set);
        ret = select(socket_fd + 1, &set, NULL, NULL, NULL);
        ERROR_CHECK(ret, -1, "select");
        if(FD_ISSET(STDIN_FILENO, &set))
        {
            char buf[60] = {0};
            ssize_t rret = read(STDIN_FILENO, buf, sizeof(buf));
            ERROR_CHECK(rret, -1, "read");
            if(rret == 0)
            {
                //EOF
                printf("disconnected!\n");
                break;
            }
            ret = send(socket_fd, buf, sizeof(buf), 0);
            ERROR_CHECK(ret, -1, "send");   
        }
        if(FD_ISSET(socket_fd, &set))
        {
            char buf[60] = {0};
            ret = recv(socket_fd, buf, sizeof(buf), 0);
            ERROR_CHECK(ret, -1, "recv");
            if(ret == 0)
            {
                printf("Server quit!\n");
                break;
            }
            printf("%s\n", buf);
        }
    }
    close(socket_fd);
}
