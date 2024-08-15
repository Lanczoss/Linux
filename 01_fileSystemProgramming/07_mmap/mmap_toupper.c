#include <linux_c_stage.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    ssize_t file_len = lseek(fd, 0, SEEK_END);
    char *p = (char*)mmap(NULL, file_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    for(int i = 0; i < file_len; i++)
    {
        p[i] = toupper(p[i]);
    }
    munmap(p, file_len);
    close(fd);
}