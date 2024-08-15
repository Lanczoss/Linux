#include <linux_c_stage.h>

int main(void)
{
    if(fork() == 0)
    {
        //子进程
        printf("我是子进程\n");
        int ret = execl("./myls", "myls",(char*)NULL);
        ERROR_CHECK(ret, -1, "exexl");
    }
    else 
    {
        //父进程回收子进程资源
        wait(NULL);
    }
}