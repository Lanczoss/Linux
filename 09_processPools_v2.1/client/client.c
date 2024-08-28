#include <cstage.h>

//向buf指向的内存中接收指定长度的数据
//第一个参数是传输用的net_fd
//第二个参数是指向的内存指针
//第三个参数是指定接收的长度
int recvn(int net_fd, void *buf, int length)
{
    char *p = (char*)buf;
    ssize_t currsize = 0;
    while(currsize < length)
    {
        ssize_t rret = recv(net_fd, p + currsize, length - currsize, 0);
        ERROR_CHECK(rret, -1, "recv");
        if(rret == 0)
        {
            return 0;
        }
        currsize += rret;
    }
    printf("recvn currsize = %ld, length = %d\n", currsize, length);
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
    ssize_t rret = recvn(socket_fd, &file_size, sizeof(off_t));

    //开辟文件大小空间
    int file_fd = open(filename, O_RDWR | O_CREAT, 0666);
    ERROR_CHECK(file_fd, -1, "open");
    ret = ftruncate(file_fd, file_size);
    ERROR_CHECK(ret, -1, "ftruncate");
    printf("file size = %ld\n", file_size);

    //mmap映射
    void *p = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_fd, 0);
    ERROR_CHECK(p, MAP_FAILED, "mmap");

    //接收文件
    rret = recvn(socket_fd, p, file_size);
    ERROR_CHECK(rret, -1, "recv");

    close(socket_fd);
    close(file_fd);
    return 0;
}
