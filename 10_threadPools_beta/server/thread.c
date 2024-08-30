#include "head.h"


//初始化子线程
//第一个参数是共有资源，第二个参数是线程个数
int initThreads(Status *status, int thread_num)
{
    //动态分配存储子线程id的数组
    pthread_t *id_list = (pthread_t*)malloc(thread_num * sizeof(pthread_t));
    ERROR_CHECK(id_list, NULL, "malloc id_list");
    for(int i = 0; i < thread_num; i++)
    {
        int ret = pthread_create(&id_list[i], NULL, threadMain, status);
        ERROR_CHECK(ret, -1, "pthread_create");
        printf("thread %d created.\n", i);
    }
    status->id_list = id_list;
    return 0;
}

void cleanLock(void *p)
{
    pthread_mutex_t *lock = (pthread_mutex_t*)p;
    printf("one thread exit.\n");
    //子线程需要退出
    int ret = pthread_mutex_unlock(lock);
    THREAD_ERROR_CHECK(ret, "unlock");
}

//子线程工作的函数
//参数是从主线程接收的参数
void *threadMain(void *p)
{
    while(1)
    {
        //接收到共有资源
        Status *s = (Status*)p;
        //开始尝试加锁并从队列取出信息
        int ret = pthread_mutex_lock(&s->lock);
        THREAD_ERROR_CHECK(ret, "lock");
        pthread_cleanup_push(cleanLock, &s->lock);
        while(s->q.head == NULL)
        {
            //没有任务使用while避免假唤醒
            ret = pthread_cond_wait(&s->cond, &s->lock);
            THREAD_ERROR_CHECK(ret, "cond_wait");
        }
        //有任务
        int net_fd;
        ret = deQueue(&s->q, &net_fd);
        THREAD_ERROR_CHECK(ret, "deQueue");
        ret = doWorker(net_fd);
        THREAD_ERROR_CHECK(ret, "doWorker");
        pthread_cleanup_pop(1);
    }
    return NULL;
}
