#include "car_model.h"
Car_Model *Car_Model::moder =nullptr;

Car_Model::Car_Model()
{

}

Car_Model *Car_Model::getInstance()
{
    if(Car_Model::moder==nullptr)
        {
            Car_Model::moder=new Car_Model;
        }
        return Car_Model::moder;
}

char **Car_Model::get_in_car(QString date)
{
    char sql[256];
    sprintf(sql,"select car_num,in_time,in_location from car where in_date='%s';", date.toStdString().c_str());
    char **qres;
    int res = Singledb_Sqlite::getInstance()->dosql(sql,qres,row,col);
    //res为查询成功
    if(res==0)
    {

        return qres;
    }
    return nullptr;
}

int Car_Model::add_car(QString car_num, QString in_date, QString in_time, QString in_location)
{
    char sql[256];
    sprintf(sql,"insert into car(car_num,in_date,in_time,in_location)values('%s','%s','%s','%s');",
            car_num.toStdString().c_str(), in_date.toStdString().c_str(),in_time.toStdString().c_str(),in_location.toStdString().c_str());
    char **qres;
    return Singledb_Sqlite::getInstance()->dosql(sql,qres,row,col);
}

int Car_Model::change_car(QString old_car_num,QString new_car_num)
{
    char sql[256];

    sprintf(sql,"update car set car_num='%s' where car_num='%s';", new_car_num.toStdString().c_str(), old_car_num.toStdString().c_str());
    char **qres;
    return Singledb_Sqlite::getInstance()->dosql(sql,qres,row,col);
}

char **Car_Model::get_date()
{
    char sql[256];
    sprintf(sql,"select distinct add_date from video;");
    char **qres;
    int res = Singledb_Sqlite::getInstance()->dosql(sql,qres,row,col);
    //res为查询成功
    if(res==0)
    {

        return qres;
    }
    return nullptr;
}


char **Car_Model::get_day()
{
    char sql[256];
    sprintf(sql,"select distinct add_day from video;");
    char **qres;
    int res = Singledb_Sqlite::getInstance()->dosql(sql,qres,row,col);
    //res为查询成功
    if(res==0)
    {

        return qres;
    }
    return nullptr;
}

char **Car_Model::get_video_by_date(QString date,int num)
{
    char sql[256];
    sprintf(sql,"select video_name from video where add_date = '%s' limit 9 offset (%d-1)*9;",date.toStdString().c_str(),num);
    char **qres;
    int res = Singledb_Sqlite::getInstance()->dosql(sql,qres,row,col);
    //res为查询成功
    if(res==0)
    {

        return qres;
    }
    return nullptr;
}

char **Car_Model::get_video_by_day(QString day,int num)
{
    char sql[256];
    sprintf(sql,"select video_name from video where add_day = '%s' limit 9 offset (%d-1)*9;",day.toStdString().c_str(),num);
    char **qres;
    int res = Singledb_Sqlite::getInstance()->dosql(sql,qres,row,col);
    //res为查询成功
    if(res==0)
    {

        return qres;
    }
    return nullptr;
}

char **Car_Model::get_last_car_num()
{
    char sql[256];
    sprintf(sql,"select car_num,in_time,in_location from car order by id desc limit 1;");
    char **qres;
    int res = Singledb_Sqlite::getInstance()->dosql(sql,qres,row,col);
    //res为查询成功
    if(res==0)
    {

        return qres;
    }
    return nullptr;
}

int Car_Model::add_video(QString video_name,QString add_date,QString add_day)
{
    char sql[256];
    sprintf(sql,"insert into video(video_name,add_date,add_day)values('%s','%s','%s');",
            video_name.toStdString().c_str(),add_date.toStdString().c_str(),add_day.toStdString().c_str());
    char **qres;
    return Singledb_Sqlite::getInstance()->dosql(sql,qres,row,col);
}

char ** Car_Model::get_video(int num)
{
    char sql[256];
       sprintf(sql,"select image_path,image_name from image limit 9 offset (%d-1)*9;",num);
       char **qres;
       //int row,col;
       Singledb_Sqlite::getInstance()->dosql(sql,qres,row,col);
       return qres;
}




