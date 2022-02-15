#ifndef SINGALDB_H
#define SINGALDB_H
#include "sqlite3.h"

#include <string>
#include <stdio.h>
class SingalDb
{
public:
    //3.公共静态方法（提供给外访问数据库单例的接口） 不用实例化对象即可调用
    static SingalDb* getInstance();
    //公共：提供外部访问
    //静态：不需要对象，用类访问即可
    void openDataBase(const char* daPath);
    void closeDataBase();
    int dosql(char* sql, char**& result, int& row, int& col);
private:
    SingalDb();//1.构造函数私有化  不能随意创建对象
    static SingalDb* mydb;//2.静态私有对象数据库单例
    //私有：不允许外部随意创建对象
    //静态：不需要对象，用类访问即可
    sqlite3* sqldb;
    char* dbPath;
    char* errmsg;
};

#endif // SINGALDB_H