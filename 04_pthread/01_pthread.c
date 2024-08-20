#include <cstage.h>

int num = 0;

void *func(void *lock)
{
    for(int i = 0; i < 20000000; i++)
    {
        int ret = pthread_mutex_lock(lock);
        THREAD_ERROR_CHECK(ret, "lock");
        num++;
        ret = pthread_mutex_unlock(lock);
        THREAD_ERROR_CHECK(ret, "unlock");
    }
    return NULL;
}

int main(void)
{
    //创建互斥锁
    pthread_mutex_t lock;
    int ret = pthread_mutex_init(&lock, NULL);
    THREAD_ERROR_CHECK(ret, "pthread_mutex_init");

   //创建子线程并传递互斥锁
   pthread_t son_id;
   ret = pthread_create(&son_id, NULL, func, &lock);
   THREAD_ERROR_CHECK(ret, "pthread_create");

   for(int i = 0; i < 20000000; i++)
   {
       ret = pthread_mutex_lock(&lock);
       THREAD_ERROR_CHECK(ret, "lock");
       num++;
       ret = pthread_mutex_unlock(&lock);
       THREAD_ERROR_CHECK(ret, "unlock");
   }
   pthread_join(son_id, NULL);
   printf("num = %d\n", num);

   //摧毁锁
   pthread_mutex_destroy(&lock);
   return 0;
}
