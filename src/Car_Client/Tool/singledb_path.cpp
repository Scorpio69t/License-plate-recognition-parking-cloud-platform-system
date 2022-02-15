#include "singledb_path.h"
#include <QFileDialog>
#include <QString>
Singledb_Path *Singledb_Path::myDB=nullptr;

Singledb_Path::Singledb_Path()
{

    QFile file("./data/set_data.txt");
    if(!file.open(QIODevice::ReadWrite))
    {
        return;
    }
    else
    {
        int line = 0;
        //判断文件是否已经读到末尾了
        while(!file.atEnd())
        {
            line++;
            QByteArray byte = file.readLine();
            char* str = byte.data();
            //读取IP数据
            if(line == 3)
            {
                this->image_path =QString(str);
            }
            else if(line == 4)
            {
                this->video_path = QString(str);
            }
            //读取端口数据
            else if(line == 5)
            {
                QString num(str);
                this->size = num.toInt();
                file.close();
                return;
            }
        }
        file.close();
    }
}
//外部公共接口 获取单例对象
Singledb_Path *Singledb_Path::getInstance()
{
    if(Singledb_Path::myDB==nullptr)
    {
        Singledb_Path::myDB=new Singledb_Path();
    }
    return Singledb_Path::myDB;
}

QString Singledb_Path::get_image_path()
{
    return image_path;
}

QString Singledb_Path::get_video_path()
{
    return video_path;
}

int Singledb_Path::get_size()
{
    return size;
}
