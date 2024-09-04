#include <cstage.h>

int main(void)
{
    MYSQL *mysql;
    mysql = mysql_init(NULL);
    printf("%s\n", mysql_error(mysql));
    mysql_real_connect(mysql, "localhost", "root", "1423", "cstage", 0, NULL, 0);
    printf("%s\n", mysql_error(mysql));
    printf("database connected.\n");

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
            printf("%s \t", row[i]);
        }
        printf("\n");
    }
    mysql_query(mysql, "select * from S");
    printf("%s\n", mysql_error(mysql));

    res = mysql_store_result(mysql);
    if(res == NULL)
    {
        printf("%s\n", mysql_error(mysql));
    }
    //获取列数
    fields = mysql_num_fields(res);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        for(int i = 0; i < fields; i++)
        {
            printf("%s \t", row[i]);
        }
        printf("\n");
    }
    mysql_query(mysql, "select * from city");
    printf("%s\n", mysql_error(mysql));

    res = mysql_store_result(mysql);
    if(res == NULL)
    {
        printf("%s\n", mysql_error(mysql));
    }
    //获取列数
    fields = mysql_num_fields(res);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        for(int i = 0; i < fields; i++)
        {
            printf("%s \t", row[i]);
        }
        printf("\n");
    }
    mysql_free_result(res);
    mysql_close(mysql);
    return 0;
}
