#include <linux_c_stage.h>

long long current_time_millis(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts); // 获取当前时间
    return (long long)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000; // 转换为毫秒
}

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    int src_fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(src_fd, -1, "open");
    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    ERROR_CHECK(dest_fd, -1, "open");
    long long time_start = current_time_millis();
    ssize_t read_count;
    char buf[100] = { 0 };
    while((read_count = read(src_fd, buf, sizeof(buf) - 1)))
    {
        write(dest_fd, buf, read_count);
    }
    long long time_end = current_time_millis();
    printf("Pass time = %lldms\n", time_end - time_start);
    close(src_fd);
    close(dest_fd);
    
    return 0;
}