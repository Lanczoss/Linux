#include <linux_c_stage.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 1);
    //找到dir2的下一个目录项
    //打开目录
    DIR *dirp = opendir(".");
    ERROR_CHECK(dirp, NULL, "opendir");
    long location = 0;
    struct dirent *dp;
    while((dp = readdir(dirp)) != NULL)
    {
        //ino_t unsigned long int
        //off_t long int;
        printf("%lu %ld %hu %u %s\n",
            dp->d_ino,
            dp->d_off,
            dp->d_reclen,
            dp->d_type,
            dp->d_name
        );
        if(strcmp("dir2", dp->d_name) == 0)
        {
            location = telldir(dirp);
        }
    }
    printf("\n");

    seekdir(dirp, location);
    dp = readdir(dirp);
    // ino_t unsigned long int
    // off_t long int;
    printf("%lu %ld %hu %u %s\n", 
        dp->d_ino, 
        dp->d_off, 
        dp->d_reclen,
        dp->d_type, 
        dp->d_name);
    closedir(dirp);
    return 0;
}
