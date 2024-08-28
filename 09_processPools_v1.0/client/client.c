#include <cstage.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    //创建socket对象
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socket_fd, -1, "socket");
    //登记ip和端口号
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(atoi(argv[2]));
    sockaddr.sin_addr.s_addr = inet_addr(argv[1]);
    //连接服务端
    int ret = connect(socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    ERROR_CHECK(ret, -1, "connect");

    //接收hello
    char buf[60] = {0};
    ssize_t rret = recv(socket_fd, buf, sizeof(buf) - 1, 0);
    ERROR_CHECK(rret, -1, "recv");
    printf("%s\n", buf);
    return 0;
}
