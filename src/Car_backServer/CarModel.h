#ifndef  CARMODEL_H
#define CARMODEL_H
#include "SingalDb.h"
#include <string>
#include <iostream>
using namespace std;
#include <list>
#include<vector>
class CarModel
{
public:
    //3.公共静态方法（提供给外访问数据库单例的接口） 不用实例化对象即可调用
    static CarModel* getInstance();
    //公共：提供外部访问
    //静态：不需要对象，用类访问即可
    int insertCarMsg(string car_num, string in_time, string local,string path);
    list<string> select_intime(string car_num);
    int deletecar();
    int updateCarmsg(string car_number, string out_time, int money);
    int totalCar();//查询停车场还有多少车
    list<string> select_carmsg(string car_number, int page);
    list<string> select_allcarmsg(string car_number,string in_time,string out_time, int page);
    list<string> select_carmsg_time(string in_time, string out_time, int page);
    list<string> select_carmsg_intime(string in_time,int page);
    list<string> select_carmsg_outtime(string out_time, int page);
    int selectVideoFrame(string name);//查询帧数
    int updateVideoFrame(string name,int count);//修改帧数
    int insertNewVideo(string name, int totalframe);//插入新的视频
private:
    CarModel();//1.构造函数私有化  不能随意创建对象
    static CarModel* cardb;//2.静态私有对象数据库单例
    //私有：不允许外部随意创建对象
    //静态：不需要对象，用类访问即可
};
#endif // ! CARMODEL_H

