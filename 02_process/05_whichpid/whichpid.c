#include <linux_c_stage.h>

/* Usage:  */
int main(void){                                  
    int fd = open("1.txt", O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    printf("process uid = %u, effective uid = %u\n", getuid(), geteuid());
    while(1);
    return 0;
}

