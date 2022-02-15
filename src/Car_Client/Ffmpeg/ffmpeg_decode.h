#ifndef FFMPEG_DECODE_H
#define FFMPEG_DECODE_H
#include <QDebug>
#include <QImage>
#include <QThread>
#include <QString>
#include <QCameraInfo>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QList>
#include <iostream>
using namespace std;
extern "C"
{
    #include "libavcodec/avcodec.h"       //编解码
    #include "libavdevice/avdevice.h"     //转码
    #include "libavfilter/avfilter.h"
    #include "libavformat/avformat.h"     //格式
    #include "libavutil/avutil.h"         //工具
    #include "libswresample/swresample.h"
    #include "libswscale/swscale.h"       //转置
}

class Ffmpeg_Decode:public QThread
{
    Q_OBJECT
public:
    static Ffmpeg_Decode* getInstance();


    //打开视频
    int open_video(QString path);
    //读取码流数据
    void decode_frame();

    QString replay_path;

    int  Seek(float pos);

    int  GetTotalTimeMsec();

    int play();
    int pause();

    int GetCurTimeMsec();
    void start(Priority = InheritPriority);
    void change_play_speed(int speed)
    {
        this->play_speed = speed;
    }


private:
    Ffmpeg_Decode();
    AVFormatContext *pformatContext; //封装格式上下文结构体
    AVCodecContext *codec;
    AVCodec *decoder;

    AVPacket *pkt;
    AVFrame *picture,*rgbpicture; //picture有损像素数据 yuvpicture去除有损数据的像素数据
    SwsContext *swscontentRGB;
    uint8_t *bufferRGB;
    QImage img;
    int play_speed = 25;
    int videoIndex = -1;
    float _totalTimeSec = 0;
    bool status = true;
    float CurTimeMsec;

    static QAtomicPointer<Ffmpeg_Decode>_instance;
    static QMutex _mutex;

    bool cacheData = false;



protected:
    void run();
signals:
    void sendImg(QImage img);
};

#endif // FFMPEG_DECODE_H
