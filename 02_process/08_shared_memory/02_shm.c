#include <linux_c_stage.h>

int main(void)
{
    key_t key = ftok("./Makefile", 10);
    ERROR_CHECK(key, -1, "ftok");
    int shmid = shmget(key, 4096, 0600 | IPC_CREAT);
    ERROR_CHECK(shmid, -1, "shmget");
    char *p = (char*)shmat(shmid, NULL, 0);
    ERROR_CHECK(p, NULL, "shmat");
    puts(p);
    int ret = shmctl(shmid, IPC_RMID, NULL);
    ERROR_CHECK(ret, -1, "shmctl");
    return 0;
}