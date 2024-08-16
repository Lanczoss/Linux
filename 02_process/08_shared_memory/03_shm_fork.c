#include <linux_c_stage.h>

int main(void)
{
    key_t key = ftok("./Makefile", 10);
    ERROR_CHECK(key, -1, "ftok");
    int shmid = shmget(key, 4096, 0600 | IPC_CREAT);
    ERROR_CHECK(shmid, -1, "shmget");
    long long *p = (long long *)shmat(shmid, NULL, 0);
    ERROR_CHECK(p, NULL, "shmat");
    p[0] = 0;
    if (fork() == 0)
    {
        // son
        for (long long i = 0; i < 10000000ll; i++)
        {
            p[0]++;
        }
    }
    else
    {
        // parent
        for (long long i = 0; i < 10000000ll; i++)
        {
            p[0]++;
        }
        wait(NULL);
        printf("%lld\n", p[0]);
        int ret = shmctl(shmid, IPC_RMID, NULL);
        ERROR_CHECK(ret, -1, "shmctl");
    }

    return 0;
}