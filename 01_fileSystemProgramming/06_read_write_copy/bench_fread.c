#include <linux_c_stage.h>

long long current_time_millis(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts); // 获取当前时间
    return (long long)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000; // 转换为毫秒
}

void copy_file(const char *src, const char *dest)
{
    FILE *src_fp = fopen(src, "rb");
    ERROR_CHECK(src, NULL, src);
    FILE *dest_fp = fopen(dest, "wb");
    if(dest_fp == NULL)
    {
        perror(dest);
        fclose(src_fp);
        exit(1);
    }
    long long time_start = current_time_millis();
    char buf[100] = { 0 };
    size_t count;
    while((count = fread(buf, sizeof(char), 100, src_fp)) > 0)
    {
        fwrite(buf, sizeof(char), count, dest_fp);
    }
    long long time_end = current_time_millis();
    printf("Pass time = %lldms\n", time_end - time_start);
    fclose(src_fp);
    fclose(dest_fp);   
}

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc, 3);
    copy_file(argv[1], argv[2]);
    return 0;
}

