#include <linux_c_stage.h>

int main()
{
    if (fork() == 0)
    {
        int fd = open("a.txt", O_RDWR);
        write(fd, "IAMSON", 6);
    }
    else
    {
        int fd = open("a.txt", O_RDWR);
        write(fd, "IAMMAIN", 7);
    }
    return 0;
}