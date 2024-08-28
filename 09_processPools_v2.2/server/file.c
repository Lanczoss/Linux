#include "head.h"


//子进程用于接收文件名的函数
//第一个参数是net_fd
//第二个参数是存文件名的空间
int recvFileName(int net_fd, char *filename)
{
    //先接收文件名长度
    ssize_t filename_size;
    ssize_t rret = recv(net_fd, &filename_size, sizeof(ssize_t), MSG_WAITALL);
    ERROR_CHECK(rret, -1, "recv filename_size");
    //再接收文件名
    rret = recv(net_fd, filename, filename_size, MSG_WAITALL);
    ERROR_CHECK(rret, -1, "recv filename");
    printf("client %d need %s\n", net_fd, filename);
    return 0;
}


//子进程用于判断是否有这个文件名
//第一个参数是传输用net_fd
//第二个参数是收到的文件名
int doesHaveFile(int net_fd, char *filename)
{
    //简易的查找：直接使用当前目录
    DIR *dirp = opendir(".");
    ERROR_CHECK(dirp, NULL, "opendir");
    struct dirent *dp;
    while((dp = readdir(dirp)) != NULL)
    {
        if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
        {
            continue;
        }
        if(strcmp(dp->d_name, filename) == 0)
        {
            printf("file found.\n");
            return 1;
        }
    }
    //这里是找不到文件
    //传递文件大小为0
    off_t file_size = 0;
    send(net_fd, &file_size, sizeof(off_t), MSG_NOSIGNAL);
    return -1;
}

//子进程用于发送数据的函数
//参数需要对端发来的文件名
int sendFile(int net_fd, char *filename)
{
    //得到文件名
    //先发送文件大小，对端可以显示进度条
    int file_fd = open(filename, O_RDWR);
    ERROR_CHECK(file_fd, -1, "open");
    //fstat可以快速获取文件大小
    struct stat st;
    memset(&st, 0, sizeof(st));
    int ret = fstat(file_fd, &st);
    ERROR_CHECK(ret, -1, "fstat");
    off_t file_size = st.st_size;
    printf("file_size = %ld\n", file_size);

    //MSG_NOSIGNAL当对方读端关闭时，不会让子进程崩溃
    ret = send(net_fd, &file_size, sizeof(off_t), MSG_NOSIGNAL);
    ERROR_CHECK(ret, -1, "send");

    //mmap零拷贝
    void *p = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_fd, 0);
    ERROR_CHECK(p, MAP_FAILED, "mmap");

    ssize_t rret = send(net_fd, p, file_size, MSG_NOSIGNAL);
    printf("rret = %ld\n", rret);
    if(rret < file_size)
    {
        printf("对端关闭\n");
        close(file_fd);
        munmap(p, file_size);
        return 0;
    }
    munmap(p, file_size);

    
    close(file_fd);
    printf("send successfully to %d!\n", net_fd);
    return 0;
}

