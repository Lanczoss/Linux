#include <cstage.h>

int main(void)
{
    MYSQL *mysql;
    mysql = mysql_init(NULL);
    printf("%s\n", mysql_error(mysql));
    mysql_real_connect(mysql, "localhost", "root", "1423", "cstage", 0, NULL, 0);
    printf("%s\n", mysql_error(mysql));
    printf("database connected.\n");

    mysql_query(mysql, "insert into student (id, name, chinese, english, math, birthday, native_place) values (NULL, '黎先', 54, 65, 76, '1999-08-2', '新疆')");
    printf("%s\n", mysql_error(mysql));

    mysql_query(mysql, "delete from student where id = 11");
    printf("%s\n", mysql_error(mysql));

    mysql_query(mysql, "update student set chinese = 100 where id = 10");
    printf("%s\n", mysql_error(mysql));

    mysql_query(mysql, "select * from student");
    printf("%s\n", mysql_error(mysql));

    MYSQL_RES *res = mysql_store_result(mysql);
    if(res == NULL)
    {
        printf("%s\n", mysql_error(mysql));
    }
    //获取列数
    unsigned int fields = mysql_num_fields(res);
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res)) != NULL)
    {
        for(int i = 0; i < fields; i++)
        {
            printf("%s\t", row[i]);
        }
        printf("\n");
    }
    mysql_free_result(res);
    mysql_close(mysql);
    return 0;
}
