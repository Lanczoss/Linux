#include <cstage.h>

typedef struct
{
    pthread_cond_t cond1;
    pthread_cond_t cond2;
    pthread_mutex_t lock;
    int flag;//如果flag等于0，b先开始
}Biglock;

void *func_t1(void *p)
{
    Biglock *b = (Biglock*)p;
    int ret = pthread_mutex_lock(&b->lock);
    THREAD_ERROR_CHECK(ret, "mutex lock");
    printf("A ->\n");
    //t1先运行
    if(b->flag == 0)
    {
        b->flag = 1;
    }

    if(b->flag == 0)
    {
        //t2先运行
        ret = pthread_cond_signal(&b->cond1);
        THREAD_ERROR_CHECK(ret, "cond1 signal");
        ret = pthread_cond_wait(&b->cond2, &b->lock);
        THREAD_ERROR_CHECK(ret, "cond2 wait");
    }
    else
    {
        //t1后运行
        ret = pthread_cond_wait(&b->cond1, &b->lock);
        THREAD_ERROR_CHECK(ret, "cond1 wait");
    }
    printf("C\n");
    ret = pthread_mutex_unlock(&b->lock);
    THREAD_ERROR_CHECK(ret, "mutex unlock");
    return NULL;
}

void *func_t2(void *p)
{
    Biglock *b = (Biglock*)p;
    int ret = pthread_mutex_lock(&b->lock);
    THREAD_ERROR_CHECK(ret, "mutex lock");
    //t2先运行
    if(b->flag == 0)
    {
        ret = pthread_cond_wait(&b->cond1, &b->lock);
        THREAD_ERROR_CHECK(ret, "cond1 wait");
        printf("B ->\n");
        ret = pthread_cond_signal(&b->cond2);
        THREAD_ERROR_CHECK(ret, "cond2 signal");
    }
    else
    {
        printf("B ->\n");
        ret = pthread_cond_signal(&b->cond1);
        THREAD_ERROR_CHECK(ret, "cond1 signal");
    }
    ret = pthread_mutex_unlock(&b->lock);
    THREAD_ERROR_CHECK(ret, "mutex unlock");
    return NULL;
}

int main(void)
{
    //定义和初始化
    Biglock b;
    int ret = pthread_mutex_init(&b.lock, NULL);
    THREAD_ERROR_CHECK(ret, "mutex init");
    ret = pthread_cond_init(&b.cond1, NULL);
    THREAD_ERROR_CHECK(ret, "cond init cond1");
    ret = pthread_cond_init(&b.cond2, NULL);
    THREAD_ERROR_CHECK(ret, "cond init cond2");
    b.flag = 0;

    pthread_t t1, t2;
    ret = pthread_create(&t1, NULL, func_t1, &b);
    THREAD_ERROR_CHECK(ret, "create t1");
    ret = pthread_create(&t2, NULL, func_t2, &b);
    THREAD_ERROR_CHECK(ret, "create t2");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&b.lock);
    pthread_cond_destroy(&b.cond1);
    pthread_cond_destroy(&b.cond2);
    return 0;
}
