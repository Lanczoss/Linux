#include <linux_c_stage.h>
#define MMAP_SIZE (1024 * 4)

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    int src_fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(src_fd, -1, "open");
    int dest_fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
    ERROR_CHECK(dest_fd, -1, "open");
    //source file size
    off_t src_size = lseek(src_fd, 0, SEEK_END);
    //allocate space
    int ret = ftruncate(dest_fd, src_size);
    ERROR_CHECK(ret, -1, "ftruncate");
    //count copied size
    off_t copied_size = 0;
    while(copied_size < src_size)
    {
        size_t curr_size = ((copied_size + MMAP_SIZE) > src_size)?
        (src_size - copied_size) : MMAP_SIZE;
        //mmap two memories
        void *src = mmap(NULL, curr_size, PROT_READ, MAP_SHARED, src_fd, copied_size);
        ERROR_CHECK(src, MAP_FAILED, "mmap src");
        void *dest = mmap(NULL, curr_size, PROT_READ | PROT_WRITE, MAP_SHARED, dest_fd, copied_size);
        ERROR_CHECK(dest, MAP_FAILED, "mmap dest");
        memcpy(dest, src, curr_size);
        //munmap
        munmap(src, curr_size);
        munmap(dest, curr_size);
        copied_size += curr_size;
    }
    close(src_fd);
    close(dest_fd);
}