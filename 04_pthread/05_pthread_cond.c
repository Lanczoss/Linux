#include <cstage.h>

typedef struct
{
    pthread_mutex_t lock;
    pthread_cond_t cond;
}Biglock;

void *func(void *p)
{
    Biglock *b = (Biglock*)p;
    int ret = pthread_mutex_lock(&b->lock);
    THREAD_ERROR_CHECK(ret, "mutex lock");

    //陷入阻塞，等待A完成，并释放锁
    ret = pthread_cond_wait(&b->cond, &b->lock);
    THREAD_ERROR_CHECK(ret, "cond wait");
    //A已经完成，开始执行B
    printf("Before B!\n");
    sleep(3);
    printf("After B\n");

    ret = pthread_mutex_unlock(&b->lock);
    THREAD_ERROR_CHECK(ret, "mutex unlock");
    return NULL;
}

int main(void)
{
    //define
    Biglock b;
    int ret = pthread_mutex_init(&b.lock, NULL);
    THREAD_ERROR_CHECK(ret, "mutex init");
    ret = pthread_cond_init(&b.cond, NULL);
    THREAD_ERROR_CHECK(ret, "cond init");

    pthread_t son_id;
    ret = pthread_create(&son_id, NULL, func, &b);
    THREAD_ERROR_CHECK(ret, "pthread create");

    //A先执行
    printf("Before A!\n");
    sleep(3);
    printf("After A\n");

    //唤醒
    ret = pthread_cond_signal(&b.cond);
    THREAD_ERROR_CHECK(ret, "cond signal");

    pthread_join(son_id, NULL);
    pthread_mutex_destroy(&b.lock);
    pthread_cond_destroy(&b.cond);
    return 0;
}
