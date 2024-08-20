#include <cstage.h>

void *func_first(void *p)
{
    printf("I am 1\n");
    return (void *)1;
}

void *func_second(void *p)
{
    printf("I am 2\n");
    return (void *)2;
}

void *func_third(void *p)
{
    printf("I am 3\n");
    return (void *)3;
}

int main(void)
{
    pthread_t first, second, third;
    int ret = pthread_create(&first, NULL, func_first, NULL);
    THREAD_ERROR_CHECK(ret, "create first");
    ret = pthread_create(&second, NULL, func_second, NULL);
    THREAD_ERROR_CHECK(ret, "create second");
    ret = pthread_create(&third, NULL, func_third, NULL);
    THREAD_ERROR_CHECK(ret, "create third");
    
    void *num1;
    void *num2;
    void *num3;
    pthread_join(first, &num1);
    pthread_join(second, &num2);
    pthread_join(third, &num3);
    printf("from first thread = %ld\n", (long)num1);
    printf("from second thread = %ld\n", (long)num2);
    printf("from third thread = %ld\n", (long)num3);
    return 0;
}
