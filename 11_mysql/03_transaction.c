#include <cstage.h>

int main(void)
{
    MYSQL *mysql;
    mysql = mysql_init(NULL);
    printf("%s\n", mysql_error(mysql));

    mysql_real_connect(mysql, "localhost", "root", "1423", "cstage", 0, NULL, 0);
    printf("%s\n", mysql_error(mysql));

    //取消自动提交事务
    mysql_autocommit(mysql, 0);

    //获取返回值检错
    int ret = mysql_query(mysql, "update student set chinese = 100 where id =6");
    if(ret !=0)
    {
        //出错 回滚事务
        mysql_rollback(mysql);
    }
    else
    {
        //正常提交事务
        mysql_commit(mysql);
    }
    mysql_autocommit(mysql, 1);
    mysql_close(mysql);
    return 0;
}
