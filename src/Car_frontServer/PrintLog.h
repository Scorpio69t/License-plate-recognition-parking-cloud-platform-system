#ifndef PRINTLOG
#define PRINTLOG
#include <iostream>
using namespace std;
#include <fstream>
class PrintLog
{
public:
    //3.公共静态方法（提供给外访问数据库单例的接口） 不用实例化对象即可调用
    static PrintLog* getInstance();
    void print_log();
    void write_login_log(char* buffer, char* fun, int size, char* number, char* name, char* pwd);//写登录注册到文件
    void write_carin_log(char* buffer, char* fun, int size, char* number, char* car_num, int fd,char *in_time);//写入场包到文件
    void write_carout_log(char* buffer, char* fun, int size, char* number, char * car_num, int fd, char *out_time );//写出场包到文件
    void write_carmsg_log(char* buffer, char* fun, int size, char* number, char* car_num, char* in_time, char* out_time);//查询
    void write_video_log(char* buffer, char* fun, int size, char* number, char* video_num, int framecount);//视频信息

    int effec_conCounts=0;//有效连接数
    int rec_packCounts=0;//接受数据包数
    int send_packCounts=0;//发送数据包数
    int userCounts=0;//用户登录数
    int get_channelCounts=0;//获取视频频道数
    int get_channel_typeCounts=0;//获取视频分类数
    int get_video_local=0;//获取视频地区
    int get_video_list=0;//获取视频列表
    int play_counts=0;//获取上传播放数据
    int play_time=0;//获取上传播放时长
private:
    PrintLog();//1.构造函数私有化
    static PrintLog* logdb;//2.静态私有对象数据库单例
};
#endif PRINTLOG

