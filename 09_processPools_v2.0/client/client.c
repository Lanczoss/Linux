#include <cstage.h>

//可以向buf指向的内存中接收指定长度(length)的数据
int recvn(int net_fd, void *buf, int length)
{
    return 0;
}

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
    //先发送文件名
    char *filename = "1.txt";

    //接收文件大小，MSG_WAITALL必须接收指定大小的数据
    //MSG_WAITALL解决半包问题
    off_t file_size = 0;
    ssize_t rret = recv(socket_fd, &file_size, sizeof(off_t), MSG_WAITALL);

    //开辟文件大小空间
    int file_fd = open(filename, O_RDWR | O_CREAT, 0666);
    ERROR_CHECK(file_fd, -1, "open");
    ret = ftruncate(file_fd, file_size);
    ERROR_CHECK(ret, -1, "ftruncate");
    printf("need file size = %ld\n", file_size);

    //循环读取文件
    while(1)
    {
        //接收数据大小
        ssize_t recv_size;
        rret = recv(socket_fd, &recv_size, sizeof(ssize_t), MSG_WAITALL);
        ERROR_CHECK(rret, -1, "recv");
        if(recv_size == 0)
        {
            //对方已退出
            break;
        }
        char buf[1024] = {0};
        rret = recv(socket_fd, buf, recv_size, MSG_WAITALL);
        printf("write size = %ld\n", rret);
        if(rret == 0)
        {
            //读取完毕
            break;
        }
        //写入文件
        rret = write(file_fd, buf, recv_size);
        ERROR_CHECK(rret, -1, "write");
    }
    close(socket_fd);
    close(file_fd);
    return 0;
}
