#include <linux_c_stage.h>

typedef struct
{
    char name[31]; // 姓名，包含一个结束字符
    int age;       // 年龄
    char gender;   // 性别（'M' = 男，'F' = 女）
} Person;

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    Person p;
    ssize_t ret = read(fd, &p, sizeof(Person));
    ERROR_CHECK(ret, -1, "read");
    printf("name = %s, age = %d, gender = %c\n", p.name, p.age, p.gender);
    close(fd);
    return 0;
}