#include <cstage.h>

typedef struct
{
    pthread_mutex_t lock;
    pthread_cond_t cond;
    //对商品数量的加锁
    int machandise;
}control_message;

void *produce_func(void *p)
{
    control_message *cm = (control_message*)p;
    while(1)
    {
        sleep(3);
        int ret = pthread_mutex_lock(&cm->lock);
        THREAD_ERROR_CHECK(ret, "produce lock");
        //避免虚假唤醒
        while(cm->machandise >= 20)
        {
            ret = pthread_cond_wait(&cm->cond, &cm->lock);
            THREAD_ERROR_CHECK(ret, "produce cond wait");
        }
        cm->machandise++;
        printf("add once, %d remains!\n", cm->machandise);
        //一个线程唤醒
        ret = pthread_cond_broadcast(&cm->cond);
        THREAD_ERROR_CHECK(ret, "prodece cond signal");
        ret = pthread_mutex_unlock(&cm->lock);
        THREAD_ERROR_CHECK(ret, "produce unlock");
    }
}

void *consume_func(void *p)
{
    control_message *cm = (control_message*)p;
    //空等待
    sleep(5);
    while(1)
    {
        int ret = pthread_mutex_lock(&cm->lock);
        THREAD_ERROR_CHECK(ret, "consume lock");
        //避免虚假唤醒
        while(cm->machandise <= 0)
        {
            ret = pthread_cond_wait(&cm->cond, &cm->lock);
            THREAD_ERROR_CHECK(ret, "consume cond wait");
        }
        cm->machandise--;
        printf("sell once, %d remains!\n", cm->machandise);
        ret = pthread_cond_broadcast(&cm->cond);
        THREAD_ERROR_CHECK(ret, "consume cond signal");
        ret = pthread_mutex_unlock(&cm->lock);
        THREAD_ERROR_CHECK(ret, "consume unlock");
    }
}

int main(void)
{
    //初始化10个商品，2个锁
    control_message cm;
    cm.machandise = 10;
    int ret = pthread_mutex_init(&cm.lock, NULL);
    THREAD_ERROR_CHECK(ret, "mutex init produce");
    ret = pthread_cond_init(&cm.cond, NULL);
    THREAD_ERROR_CHECK(ret, "cond init");

    //创建2个生产者，3个消费者
    pthread_t p1, p2, c1, c2, c3;
    ret = pthread_create(&p1, NULL, produce_func, &cm);
    THREAD_ERROR_CHECK(ret, "create p1");
    ret = pthread_create(&p2, NULL, produce_func, &cm);
    THREAD_ERROR_CHECK(ret, "create p2");
    ret = pthread_create(&c1, NULL, consume_func, &cm);
    THREAD_ERROR_CHECK(ret, "create c1");
    ret = pthread_create(&c2, NULL, consume_func, &cm);
    THREAD_ERROR_CHECK(ret, "create c2");
    ret = pthread_create(&c3, NULL, consume_func, &cm);
    THREAD_ERROR_CHECK(ret, "create c3");

    //等待所有子线程
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(c1, NULL);
    pthread_join(c2, NULL);
    pthread_join(c3, NULL);

    //销毁所有资源
    pthread_mutex_destroy(&cm.lock);
    pthread_cond_destroy(&cm.cond);
    return 0;
}
