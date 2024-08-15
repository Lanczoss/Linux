#include <linux_c_stage.h>

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
    char buf[1024] = { 0 };
    size_t count;
    while((count = fread(buf, sizeof(char), 1024, src_fp)) > 0)
    {
        fwrite(buf, sizeof(char), count, dest_fp);
    }
    fclose(src_fp);
    fclose(dest_fp);
    
}

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc, 3);
    copy_file(argv[1], argv[2]);
    return 0;
}

