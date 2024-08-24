#include <cstage.h>

int main(void)
{
    //int为4B
    //十六进制的64为十进制的97
    //小端法输出为英文字母d
    int num = 0x61626364;
    char *c = (char*)&num;
    printf("host = %c\n", *c);

    //转为大端法
    int ret = htonl(num);
    c = (char*)&ret;
    printf("net = %c\n", *c);
    return 0;
}
