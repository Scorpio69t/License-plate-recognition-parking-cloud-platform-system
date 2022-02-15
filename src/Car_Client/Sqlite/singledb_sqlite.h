#ifndef SINGLEDB_SQLITE_H
#define SINGLEDB_SQLITE_H
#include <sqlite3.h>
#include <iostream>
using namespace std;
class Singledb_Sqlite
{
public:
    ~Singledb_Sqlite();
    //外部公共接口 获取model对象
    static Singledb_Sqlite *getInstance();
    //static void releaseInstence();
    //数据库执行
    int dosql(char *sql,char **&result,int &row, int &col);
    //打开数据库
    void openDataBase(const char *dbPath);
    //关闭数据库
    void closeDataBase();

private:
    Singledb_Sqlite();//构造私有化
    static Singledb_Sqlite *myDB;//静态私成员变量
    sqlite3 *sqldb;
    char *errmsg;  //用来存储错误信息字符串

};

#endif // SINGLEDB_SQLITE_H
