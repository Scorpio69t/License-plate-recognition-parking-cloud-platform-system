#ifndef CAR_MODEL_H
#define CAR_MODEL_H
#include <QString>
#include <QDebug>
#include "Sqlite/singledb_sqlite.h"
#include <iostream>
using namespace std;


class Car_Model
{
public:

    //外部公共接口 获取model对象
    static Car_Model *getInstance();
    //获取车辆入场信息
    char ** get_in_car(QString date);

    int add_car(QString car_num,QString in_date,QString in_time,QString in_location);

    int change_car(QString old_car_num,QString new_car_num);

    char **get_date();

    char **get_day();

    char **get_video_by_date(QString date,int num);

    char **get_video_by_day(QString day,int num);

    char **get_last_car_num();

    int add_video(QString video_name,QString add_date,QString add_day);

    char ** get_video(int num);

    int get_row()
    {
        return row;
    }
    int get_col()
    {
        return col;
    }
private:
    Car_Model();//构造私有化
    static Car_Model *moder; //静态私成员变量
    int row,col;

};

#endif // CAR_MODEL_H
