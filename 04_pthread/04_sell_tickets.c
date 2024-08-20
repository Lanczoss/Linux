#include <cstage.h>

typedef struct
{
    int ticket_num;
    pthread_mutex_t lock;
    int flag;
}Message;

void *sell(void *m)
{
    Message *pm = (Message*)m;
    //random seed
    srand(time(NULL));
    while(1)
    {
        int ret = pthread_mutex_lock(&pm->lock);
        THREAD_ERROR_CHECK(ret, "lock");
        if(pm->ticket_num == 0 && pm->flag ==1)
        {
            ret = pthread_mutex_unlock(&pm->lock);
            THREAD_ERROR_CHECK(ret, "unlock");
            break;
        }
        if(pm->ticket_num > 0 && rand() % 1 < 0.5)
        {
            pm->ticket_num--;
            printf("sell once, %d remain\n", pm->ticket_num);
        }
        ret = pthread_mutex_unlock(&pm->lock);
        THREAD_ERROR_CHECK(ret, "unlock");
    }
    return NULL;
}

void *add(void *m)
{
    Message *pm = (Message*)m;
    while(1)
    {
        int ret = pthread_mutex_lock(&pm->lock);
        THREAD_ERROR_CHECK(ret, "lock");
        if(pm->ticket_num <= 5 && pm->flag == 0)
        {
            pm->ticket_num += 10;
            printf("add 10 tickets!\n");
            pm->flag = 1;
            ret = pthread_mutex_unlock(&pm->lock);
            THREAD_ERROR_CHECK(ret, "unlock");
            break;
        }
        ret = pthread_mutex_unlock(&pm->lock);
        THREAD_ERROR_CHECK(ret, "unlock");
    }
    return NULL;
}

int main(void)
{
    //传递的票数和互斥锁
    Message m;
    m.ticket_num = 20;
    m.flag = 0;
    int ret = pthread_mutex_init(&m.lock, NULL);
    THREAD_ERROR_CHECK(ret, "mutex init");

    //两个线程
    pthread_t sell_thread, add_thread;
    ret = pthread_create(&sell_thread, NULL, sell, &m);
    THREAD_ERROR_CHECK(ret, "pthread create sell");
    ret = pthread_create(&add_thread, NULL, add, &m);
    THREAD_ERROR_CHECK(ret, "pthread create add");

    ret = pthread_join(sell_thread, NULL);
    THREAD_ERROR_CHECK(ret, "join sell thread");
    ret = pthread_join(add_thread, NULL);
    THREAD_ERROR_CHECK(ret, "join add thread");

    ret = pthread_mutex_destroy(&m.lock);
    THREAD_ERROR_CHECK(ret, "mutex destory");
    return 0;
}
