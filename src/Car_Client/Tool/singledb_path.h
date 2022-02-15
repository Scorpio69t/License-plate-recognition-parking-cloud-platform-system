#ifndef SINGLEDB_PATH_H
#define SINGLEDB_PATH_H

#include <iostream>
#include <unistd.h>
#include <QString>
using namespace std;
class Singledb_Path
{
public:
    //外部公共接口 获取model对象
    static Singledb_Path *getInstance();

    QString get_image_path();
    QString get_video_path();
    int get_size();

private:
    Singledb_Path();//构造私有化
    static Singledb_Path *myDB;//静态私成员变量
    QString image_path;
    QString video_path;
    int size;
};

#endif // SINGLEDB_PATH_H
