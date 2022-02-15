#include "PrintLog.h"
PrintLog *PrintLog::logdb;
PrintLog::PrintLog()
{

}
PrintLog* PrintLog::getInstance()
{
    if (PrintLog::logdb == nullptr)
    {
        PrintLog::logdb = new PrintLog();
    }
    return PrintLog::logdb;
}
void PrintLog::print_log()
{
    printf("以下内容：\n");
    cout << "以下为实时日志" << endl;
    cout << "有效连接数：" << this->effec_conCounts << endl;
    cout << "接受数据包：" << this->rec_packCounts << endl;
    cout << "发送数据包：" << this->send_packCounts << endl;
    cout << "用户登录数：" << this->userCounts << endl;
    //cout << "获取视频频道：" << this->get_channelCounts << endl;
    //cout << "获取视频分类：" << this->get_channel_typeCounts << endl;
    //cout << "获取视频地区：" << this->get_video_local << endl;
    //cout << "获取视频列表：" << this->get_video_list << endl;
    cout << "上传播放数据：" << this->play_counts << endl;
    //cout << "上传播放时长：" << this->play_time << endl;
    cout << "--------------------------------------------------------------------" << endl;
}
void  PrintLog::write_login_log(char* buffer, char* fun, int size, char* number, char* name, char* pwd)
{
    time_t nowTime;
    nowTime = time(NULL);
    struct tm* sysTime = localtime(&nowTime);
    char nowtime_buf[50] = { 0 };
    //char nowtime[50] = { 0 };
    sprintf(nowtime_buf, "./log/%d-%02d-%02d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);
    cout << nowtime_buf << endl;
    ofstream out(nowtime_buf, ofstream::out | ofstream::in| ofstream::app);
    out << "时间：" << buffer << endl;
    out << "功能：" << fun << endl;
    out << "数据包大小：" << size << endl;
    out << "数据包流水号：" << number << endl;
    out << "用户名：" << name << endl;
    out << "密码：" << pwd << endl;
    out << endl;
}
void  PrintLog::write_carin_log(char* buffer, char* fun, int size, char* number, char* car_num,int fd,char *in_time)
{
    time_t nowTime;
    nowTime = time(NULL);
    struct tm* sysTime = localtime(&nowTime);
    char nowtime_buf[50] = { 0 };
    //char nowtime[50] = { 0 };
    sprintf(nowtime_buf, "./log/%d-%02d-%02d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);
    ofstream out(nowtime_buf, ofstream::out | ofstream::in | ofstream::app);
    out << "时间：" << buffer << endl;
    out << "功能：" << fun << endl;
    out << "数据包大小：" << size << endl;
    out << "数据包流水号：" << number << endl;
    out << "车牌号：" << car_num << endl;
    out<< "入场时间：" << in_time << endl;
    out << "套接字fd=:" << fd << endl;
    out << endl;
}
void PrintLog::write_carout_log(char* buffer, char* fun, int size, char *number,char* car_num, int fd, char* out_time)
{
    time_t nowTime;
    nowTime = time(NULL);
    struct tm* sysTime = localtime(&nowTime);
    char nowtime_buf[50] = { 0 };
    //char nowtime[50] = { 0 };
    sprintf(nowtime_buf, "./log/%d-%02d-%02d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);
    ofstream out(nowtime_buf, ofstream::out | ofstream::in | ofstream::app);
    out << "时间：" << buffer << endl;
    out << "功能：" << fun << endl;
    out << "数据包大小：" << size << endl;
    out << "数据包流水号：" << number << endl;
    out << "车牌号：" << car_num << endl;
    out << "出场时间：" << out_time << endl;
    out << "套接字fd=:" << fd << endl;
    out << endl;
}
void PrintLog::write_carmsg_log(char* buffer, char* fun, int size, char* number, char* car_num, char* in_time, char* out_time)//查询
{
    time_t nowTime;
    nowTime = time(NULL);
    struct tm* sysTime = localtime(&nowTime);
    char nowtime_buf[50] = { 0 };
    //char nowtime[50] = { 0 };
    sprintf(nowtime_buf, "./log/%d-%02d-%02d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);
    ofstream out(nowtime_buf, ofstream::out | ofstream::in | ofstream::app);
    out << "时间：" << buffer << endl;
    out << "功能：" << fun << endl;
    out << "数据包大小：" << size << endl;
    out << "数据包流水号：" << number << endl;
    out << "车牌号：" << car_num << endl;
    out << "入场时间：" << in_time<< endl;
    out << "出场时间:" << out_time << endl;
    out << endl;
}
void PrintLog::write_video_log(char* buffer, char* fun, int size, char* number, char* video_num, int framecount)//视频信息
{
    time_t nowTime;
    nowTime = time(NULL);
    struct tm* sysTime = localtime(&nowTime);
    char nowtime_buf[50] = { 0 };
    //char nowtime[50] = { 0 };
    sprintf(nowtime_buf, "./log/%d-%02d-%02d.txt", sysTime->tm_year + 1900, sysTime->tm_mon + 1, sysTime->tm_mday);
    ofstream out(nowtime_buf, ofstream::out | ofstream::in | ofstream::app);
    out << "时间：" << buffer << endl;
    out << "功能：" << fun << endl;
    out << "数据包大小：" << size << endl;
    out << "数据包流水号：" << number << endl;
    out << "视频名称：" << video_num << endl;
    out << "视频总帧数：" << framecount << endl;
    out << endl;
}