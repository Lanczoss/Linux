#include <linux_c_stage.h>
const char months[][10] = {"Mon", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void set_type_mode(mode_t mode, char *mode_str) {
    switch (mode & S_IFMT) {
    case S_IFBLK:
        mode_str[0] = 'b';
        break;
    case S_IFCHR:
        mode_str[0] = 'c';
        break;
    case S_IFDIR:
        mode_str[0] = 'd';
        break;
    case S_IFIFO:
        mode_str[0] = 'p';
        break;
    case S_IFLNK:
        mode_str[0] = 'l';
        break;
    case S_IFREG:
        mode_str[0] = '-';
        break;
    case S_IFSOCK:
        mode_str[0] = 's';
        break;
    default:
        mode_str[0] = '?';
        break;
    }
    if (mode & 0400) {
        mode_str[1] = 'r';
    } else {
        mode_str[1] = '-';
    }
    if (mode & 0200) {
        mode_str[2] = 'w';
    } else {
        mode_str[2] = '-';
    }
    if (mode & 0100) {
        mode_str[3] = 'x';
    } else {
        mode_str[3] = '-';
    }
    if (mode & 040) {
        mode_str[4] = 'r';
    } else {
        mode_str[4] = '-';
    }
    if (mode & 020) {
        mode_str[5] = 'w';
    } else {
        mode_str[5] = '-';
    }
    if (mode & 010) {
        mode_str[6] = 'x';
    } else {
        mode_str[6] = '-';
    }
    if (mode & 04) {
        mode_str[7] = 'r';
    } else {
        mode_str[7] = '-';
    }
    if (mode & 02) {
        mode_str[8] = 'w';
    } else {
        mode_str[8] = '-';
    }
    if (mode & 01) {
        mode_str[9] = 'x';
    } else {
        mode_str[9] = '-';
    }
}

void set_time(time_t t, char *time_buf)
{
    struct tm *lt = localtime(&t);
    sprintf(time_buf, "%s %2d %d:%d",
        months[lt->tm_mon],
        lt->tm_mday,
        lt->tm_hour, 
        lt->tm_min);
}

int main(int argc, char *argv[]) {
    char *path;
    if (argc == 1) {
        path = ".";
    } else if (argc == 2) {
        path = argv[1];
    } else {
        fprintf(stderr, "args numbers wrong.\n");
        exit(1);
    }

    // 改变工作目录到指定目录
    int ret = chdir(path);
    ERROR_CHECK(ret, -1, "chdir");

    // 打开目录
    DIR *dirp = opendir(".");
    ERROR_CHECK(dirp, NULL, "opendir");
    struct dirent *dp;
    // 循环每一个目录项
    while ((dp = readdir(dirp)) != NULL) {
        // 每一个目录项都有一个stat
        struct stat stat_buf;
        int ret = stat(dp->d_name, &stat_buf);
        ERROR_CHECK(ret, -1, "stat");

        // 解决文件类型和文件权限显示
        char mode_buf[1024] = {0};
        set_type_mode(stat_buf.st_mode, mode_buf);

        // 解决时间显示
        char time_buf[1024] = {0};
        set_time(stat_buf.st_mtim.tv_sec, time_buf);

        printf("%s %2lu %s %s %5lu %s %s\n",
            mode_buf,
            stat_buf.st_nlink,
            getpwuid(stat_buf.st_uid)->pw_name,
            getgrgid(stat_buf.st_gid)->gr_name,
            stat_buf.st_size,
            time_buf,
            dp->d_name
            );
    }
    closedir(dirp);
    return 0;
}
