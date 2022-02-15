#include "CarModel.h"
#include <iostream>
using namespace std;
CarModel* CarModel::cardb;

CarModel::CarModel()
{

}
CarModel* CarModel::getInstance()
{
    if (CarModel::cardb == nullptr)
    {
        CarModel::cardb = new CarModel();
    }
    return CarModel::cardb;
}
int CarModel::insertCarMsg(string car_num, string in_time, string local, string path)
{
	//sql语句拼接
	char sql[256];
	sprintf(sql, "insert into tbl_msg values(null,'%s','null','%s','null','%s','null',0,1,'%s',0)", 
		    car_num.c_str(),in_time.c_str(),path.c_str(),local.c_str());
	//数据准备
	char** qres;
	int row, col;
	//打开数据库
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		return 0;
	}
	else {
		return  -1;
	}
}
int CarModel::deletecar()
{
	//sql语句拼接
	char sql[256];
	sprintf(sql, "delete from tbl_msg ");
	//数据准备
	char** qres;
	int row, col;
	//打开数据库
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		return 0;
	}
	else {
		return  -1;
	}
}
list<string> CarModel::select_intime(string car_num)
{
	//sql语句拼接
	char sql[256];
	sprintf(sql, "select goin_time,vip from tbl_msg where car_number='%s' and status=1",car_num.c_str());
	//数据准备
	char** qres;
	int row, col;
	list<string> list;
	//打开数据库
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		if (row == 0)
		{
			return list;
		}
		else {
			for (int i = col; i < (row + 1) * col; i++)
			{
				list.push_back(qres[i]);
			}
			return list;
		}
	}
}
int CarModel::updateCarmsg(string car_number,string out_time,int money)
{
	//sql语句拼接
	char sql[256];
	sprintf(sql, "update tbl_msg set status=0,take_money=%d,goout_time='%s' where car_number='%s'",money, out_time.c_str(),car_number.c_str());
	//数据准备
	char** qres;
	int row, col;
	list<string> list;
	//打开数据库
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		return 0;
	}
	else {
		return  -1;
	}
}
int CarModel::totalCar()//查询停车场还有多少车
{
	//sql语句拼接
	char sql[256];
	sprintf(sql, "select COUNT(*) from tbl_msg where status=1");
	//数据准备
	char** qres;
	int row, col;
	list<string> list;
	//打开数据库
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		if (row == 0)
		{
			return 0;
		}
		else {
			return atoi(qres[1]);
		}
	}
}
list<string> CarModel::select_carmsg(string car_number,int page)
{
	//sql语句拼接
	char sql[256];
	sprintf(sql, "select car_number,goin_time,goout_time,take_money from tbl_msg where car_number='%s' ORDER BY carmsg_id ASC limit 4 OFFSET %d", car_number.c_str(),page);
	cout << "sql=" << sql << endl;
	//数据准备
	char** qres;
	int row, col;
	list<string> list;
	//打开数据库
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		if (row == 0)
		{
			return list;
		}
		else {
			for (int i = col; i < (row + 1) * col; i++)
			{
				list.push_back(qres[i]);
			}
			return list;
		}
	}
}
list<string> CarModel::select_allcarmsg(string car_number, string in_time, string out_time, int page)
{
	//sql语句拼接
	char sql[256];
	sprintf(sql, "select car_number,goin_time,goout_time,take_money from tbl_msg where goin_time between '%s' and '%s' and car_number='%s' ORDER BY carmsg_id ASC limit 4 OFFSET %d", in_time.c_str(),out_time.c_str(),car_number.c_str(),page);
	cout << "sql=" << sql << endl;
	//数据准备
	char** qres;
	int row, col;
	list<string> list;
	//打开数据库
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		if (row == 0)
		{
			return list;
		}
		else {
			for (int i = col; i < (row + 1) * col; i++)
			{
				list.push_back(qres[i]);
			}
			return list;
		}
	}
}
list<string> CarModel::select_carmsg_time(string in_time, string out_time, int page)
{
	//sql语句拼接
	char sql[256];
	sprintf(sql, "select car_number,goin_time,goout_time,take_money from tbl_msg where goin_time between '%s' and '%s' ORDER BY carmsg_id ASC limit 4 OFFSET %d", in_time.c_str(), out_time.c_str(), page);
	//数据准备
	char** qres;
	int row, col;
	list<string> list;
	//打开数据库
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		if (row == 0)
		{
			return list;
		}
		else {
			for (int i = col; i < (row + 1) * col; i++)
			{
				list.push_back(qres[i]);
			}
			return list;
		}
	}
}
list<string> CarModel::select_carmsg_intime(string in_time, int page)
{
	//sql语句拼接
	char sql[256];
	sprintf(sql, "select car_number,goin_time,goout_time,take_money from tbl_msg where goin_time >= '%s' ORDER BY carmsg_id ASC limit 4 OFFSET %d", in_time.c_str(), page);
	//数据准备
	char** qres;
	int row, col;
	list<string> list;
	//打开数据库
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		if (row == 0)
		{
			return list;
		}
		else {
			for (int i = col; i < (row + 1) * col; i++)
			{
				list.push_back(qres[i]);
			}
			return list;
		}
	}
}
list<string> CarModel::select_carmsg_outtime(string out_time, int page)
{
	//sql语句拼接
	char sql[256];
	sprintf(sql, "select car_number,goin_time,goout_time,take_money from tbl_msg where goout_time <= '%s' ORDER BY carmsg_id ASC limit 4 OFFSET %d", out_time.c_str(), page);
	//数据准备
	char** qres;
	int row, col;
	list<string> list;
	//打开数据库
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		if (row == 0)
		{
			return list;
		}
		else {
			for (int i = col; i < (row + 1) * col; i++)
			{
				list.push_back(qres[i]);
			}
			return list;
		}
	}
}
int CarModel::updateVideoFrame(string name,int count)
{
	//sql语句拼接
	char sql[256];
	sprintf(sql, "update tbl_record set record_nowframes=%d where record_name='%s'",count,name.c_str());
	//数据准备
	char** qres;
	int row, col;
	//打开数据库
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		return 0;
	}
	else {
		return  -1;
	}
}
int CarModel::selectVideoFrame(string name)
{
	//sql语句拼接
	char sql[256];
	sprintf(sql, "select record_nowframes from tbl_record where record_name='%s'",name.c_str());
	//数据准备
	char** qres;
	int row, col;
	//打开数据库
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		if (row == 0)
		{
			return 0;
		}
		else {
			return atoi(qres[1]);
		}
	}
}
int CarModel::insertNewVideo(string name, int totalframe)
{
	//sql语句拼接
	char sql[256];
	sprintf(sql, "insert into tbl_record values(null,'%s',0,%d)",
		name.c_str(), totalframe);
	//数据准备
	char** qres;
	int row, col;
	//打开数据库
	int res = SingalDb::getInstance()->dosql(sql, qres, row, col);
	if (res == 0)
	{
		return 0;
	}
	else {
		return  -1;
	}
}