#include <cstage.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    struct addrinfo *res;
    int ret = getaddrinfo(argv[1], NULL, NULL, &res);
    ERROR_CHECK(ret, -1, "getaddrinfo");
    for(struct addrinfo *p = res; p != NULL; p = p->ai_next)
    {
        //获取struct sockaddr
        struct sockaddr *tmp = p->ai_addr;
       //只要通用类型的ipv4
       if(tmp->sa_family == AF_INET)
       {
           //强转为struct sockaddr_in类型
           struct sockaddr_in *tmp_v4 = (struct sockaddr_in*)tmp;
           //将地址转换为点分十进制字符串
           char *address = inet_ntoa(tmp_v4->sin_addr);
           printf("ip = %s\n", address);
       }
    }
    freeaddrinfo(res);
}
