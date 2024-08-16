#include <linux_c_stage.h>

int main(void)
{
    key_t key = ftok("./Makefile", 10);
    ERROR_CHECK(key, -1, "ftok");
    int shmid = shmget(key, 4096, 0600 | IPC_CREAT);
    ERROR_CHECK(shmid, -1, "shmget");
    char *p = (char*)shmat(shmid, NULL, 0);
    ERROR_CHECK(p, NULL, "shmat");
    strcpy(p, "How are you");
    shmdt(p);
    return 0;
}