#include "head.h"


//socketpair得到的local_socket
//通过本地socket发送文件描述符
//第一个参数是通信用本地socket
//第二个参数是要发送的文件描述符
int sendMsg(int local_socket, int net_fd)
{
    //头部信息
    struct msghdr msg;
    bzero(&msg, sizeof(msg));
    //正文部分
    //IO向量数组
    struct iovec vec[1];
    char *str = "hello";
    vec[0].iov_base = str;
    vec[0].iov_len = strlen(str);
    //头部控制信息部分，可变长度需要malloc
    struct cmsghdr *cmsg = (struct cmsghdr*)malloc(CMSG_LEN(sizeof(int)));
    ERROR_CHECK(cmsg, NULL, "malloc");
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    //cmsg的cms_data用CMS_DATA可获得指针
    void *p = CMSG_DATA(cmsg);
    int *fd = (int *)p;
    *fd = net_fd;
    //存入头部信息中
    msg.msg_iov = vec;
    msg.msg_iovlen = 1;//数组长度
    msg.msg_control = cmsg;
    msg.msg_controllen = CMSG_LEN(sizeof(int));
    //发送
    int ret = sendmsg(local_socket, &msg, 0);
    ERROR_CHECK(ret, -1, "sendmsg");
    free(cmsg);
    return 0;
}

//通过本地socket接收文件描述符
//第一个参数是通信用本地socket
//第二个传入传出参数是接收的netfd
int recvMsg(int local_socket, int *net_fd)
{
    struct msghdr msg;
    bzero(&msg, sizeof(msg));
    //正文部分
    char str[60] = {0};
    struct iovec vec[1];
    vec[0].iov_base = str;
    vec[0].iov_len = sizeof(str);
    msg.msg_iov = vec;
    msg.msg_iovlen = 1;
    //控制部分
    struct cmsghdr *cmsg = (struct cmsghdr*)malloc(CMSG_LEN(sizeof(int)));
    ERROR_CHECK(cmsg, NULL, "malloc");
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    msg.msg_control = cmsg;
    msg.msg_controllen = CMSG_LEN(sizeof(int));
    int ret = recvmsg(local_socket, &msg, 0);
    ERROR_CHECK(ret, -1, "recvmsg");
    printf("recv net_fd success!\n");
    //取出文件描述符
    void *p = CMSG_DATA(cmsg);
    int *fd = (int*)p;
    *net_fd = *fd;
    free(cmsg);
    return 0;
}
