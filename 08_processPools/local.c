#include "head.h"


//socketpair得到的local_socket
//通过本地socket发送文件描述符
//第一个参数是通信用本地socket
//第二个参数是要发送的文件描述符
int sendMsg(int local_socket, int net_fd)
{
    //头部信息
    struct msghdr msg;
    //正文部分
    //IO向量数组
    struct iovec vec[1];
    char str[60] = {0};
    vec[0].iov_base = str;
    vec[0].iov_len = strlen(str);
    //头部控制信息部分，可变长度需要malloc
    struct cmsghdr *cmsg = (struct cmsghdr*)malloc(CMSG_LEN(sizeof(int)));
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_level = SOL_SOCKET;
    //cmsg的cms_data用CMS_DATA可获得指针
    void *p = CMSG_DATA(cmsg);
    int *fd = (int *)p;
    *fd = net_fd;
    //存入头部信息中
    msg.msg_iov = vec;
    msg.msg_iovlen = 1;//数组长度
    msg.msg_control = cmsg;
    msg.msg_control = CMSG_LEN(cmsg);
    //发送
    int ret = sendmsg(local_socket, &msg, 0);
    if(ret == -1)
    {
        perror("sendmsg");
        return -1;
    }
    return 0;
}

//通过本地socket接收文件描述符
//第一个参数是通信用本地socket
//第二个传入传出参数是接收的netfd
int recvMsg(int local_socket, int *net_fd);
