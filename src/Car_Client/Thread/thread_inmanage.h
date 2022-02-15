#ifndef THREAD_INMANAGE_H
#define THREAD_INMANAGE_H

#include <QThread>
#include <iostream>
#include <QPainter>
#include <QObject>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
#define MANUAL 1 //手动录制
#define AUTO   2 //自动录制
#define STOP   0 //无录制
class Thread_Inmanage : public QThread
{
    Q_OBJECT
public:
    Thread_Inmanage();
    void video_save();
    static int state;
private:
    VideoCapture capture;
    Mat picture;
    QImage img;
    Mat img_RGB;
    VideoWriter write;
    //int state;
    int counter;
    int first;
    QString time;
    QString date;
    QString day;
    QString path;
    CascadeClassifier cascade;
    CascadeClassifier mancascade;
    void detectCarDraw(Mat &img,CascadeClassifier &cascade,double scale);
signals:
    void butt_enable();
protected:
    void run();

signals:
    void sendImg(QImage img);
    void sendMat(Mat mat);
};

#endif // THREAD_INMANAGE_H
