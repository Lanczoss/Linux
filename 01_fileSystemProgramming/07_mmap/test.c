#include <linux_c_stage.h>

int main(void)
{
    long page_size = sysconf(_SC_PAGE_SIZE);
    printf("page size = %ld\n", page_size);
    return 0;
}