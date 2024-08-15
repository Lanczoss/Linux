#include <linux_c_stage.h>

int main(int argc, char *argv[])
{
    char *path = NULL;
    if(argc == 1)
    {
        path = ".";
    }
    else if(argc == 2)
    {
        path = argv[1];
    }
    else
    {
        fprintf(stderr, "args number wrong.\n");
        exit(1);
    }

    //打开目录
    DIR *dirp = opendir(path);
    ERROR_CHECK(dirp, NULL, "opendir");
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
    }
    closedir(dirp);
    return 0;
}
