#include <linux_c_stage.h>

int main(void)
{
    DIR *dirp = opendir(".");
    ERROR_CHECK(dirp, -1, "opendir");
    struct dirent *dp;
    while((dp = readdir(dirp)) != 0)
    {
        printf("%s  ", dp->d_name);
    }
    printf("\n");
    return 0;
}