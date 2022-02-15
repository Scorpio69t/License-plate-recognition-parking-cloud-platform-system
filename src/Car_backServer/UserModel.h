#ifndef USERMODEL_H
#define USERMODEL_H
#include <iostream>
using namespace std;
#include <string>
#include <list>
#include"SingalDb.h"
class UserModel
{
public:
    //3.公共静态方法（提供给外访问数据库单例的接口） 不用实例化对象即可调用
    static UserModel* getInstance();
    int insertUser(string name, string pwd);//新增用户
    int searchUser(string name, string pwd);//查找用户
    int changeState(int state,string name);//修改登录状态下线状态
    
private:
    UserModel();//1.构造函数私有化
    static UserModel* userdb;//2.静态私有对象数据库单例
};
#endif // USERMODEL_H