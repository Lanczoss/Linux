#include <linux_c_stage.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    char c;
    while(1)
    {
        ssize_t read_count = read(fd, &c, sizeof(char));
        if(read_count == 0)
        {
            break;
        }
        char tmp = toupper(c);
        lseek(fd, -1, SEEK_CUR);
        write(fd, &tmp, sizeof(char));
    }
    close(fd);
}