#include "singledb_sqlite.h"



Singledb_Sqlite *Singledb_Sqlite::myDB=nullptr;
//单例打开数据库路径
Singledb_Sqlite::Singledb_Sqlite()
{
    this->openDataBase("./data/SQL");
}
//外部公共接口 获取单例对象
Singledb_Sqlite *Singledb_Sqlite::getInstance()
{
    if(Singledb_Sqlite::myDB==nullptr)
    {
        Singledb_Sqlite::myDB=new Singledb_Sqlite();
    }

    return Singledb_Sqlite::myDB;

}
//数据库执行
int Singledb_Sqlite::dosql(char *sql, char **&result, int &row, int &col)
{
    int res =sqlite3_get_table(sqldb, sql, &result, &row, &col, &errmsg);
    if(res!=SQLITE_OK)
    {
        cout<<sqlite3_errmsg(sqldb)<<endl;
        return res;
    }
    cout<<row<<"-----"<<col<<endl;
    return 0;

}
//打开数据库
void Singledb_Sqlite::openDataBase(const char *dbPath)
{
    int res = sqlite3_open(dbPath, &sqldb);
    if(res != SQLITE_OK)
    {
        cout<<sqlite3_errmsg(sqldb)<<endl;
        cout<<sqlite3_errcode(sqldb)<<endl;
    }


}
//关闭数据库
void Singledb_Sqlite::closeDataBase()
{
    int res = sqlite3_close(sqldb);
    if(res != SQLITE_OK)
    {
        cout<<sqlite3_errmsg(sqldb)<<endl;
        cout<<sqlite3_errcode (sqldb) <<endl;
    }
}
