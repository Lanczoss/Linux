#include <linux_c_stage.h>

void tree_recursion(char *path, int countspace)
{
    DIR *dirp = opendir(path);
    ERROR_CHECK(dirp, NULL, "opendir");
    struct dirent *dp;
    while  ((dp = readdir(dirp)) != NULL)
    {
        if  (strcmp(".", dp->d_name) == 0)
        {
            continue;
        }
        if  (strcmp("..", dp->d_name) == 0)
        {
            continue;
        }
        int cnt = countspace;
        while  (cnt != 1)
        {
            printf("   ");
            cnt--;
        }
        printf("└");
        for  (int i = 0 ; i <  2; i++)
        {
            printf("─");
        }
        printf("%s\n", dp->d_name);
        if  (dp->d_type == DT_DIR)
        {
            char pathname[1024] = {0};
            sprintf(pathname, "%s%s%s", path, "/", dp->d_name);
            tree_recursion(pathname, countspace + 1);
        }
    }
    closedir(dirp);
}

int main(int argc, char *argv[])
{
    char *path;
    if  (argc == 1)
    {
        path = ".";
    }
    else if  (argc == 2)
    {
        path = argv[1];
    }
    else
    {
        fprintf(stderr, "args numbers wrong.\n");
        exit(1);
    }
    printf(".\n");
    tree_recursion(path, 1);
    return 0;
}