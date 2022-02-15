#ifndef THREAD_OUT_H
#define THREAD_OUT_H

#include <QThread>
#include <iostream>
#include <QPainter>
#include <QObject>
#include <opencv2/opencv.hpp>

#include <easypr.h>
using namespace std;
using namespace cv;
using namespace easypr;
class Thread_Out : public QThread
{
    Q_OBJECT
public:
    Thread_Out();
    void detectCarDraw(Mat &img,CascadeClassifier &cascade,double scale);
private:
    VideoCapture capture;
    Mat picture;
    CascadeClassifier cascade;
    QImage img;
    Mat img_RGB;
protected:
    void run();
signals:
    void sendImg(QImage img);
    void sendMat(Mat mat);
};

#endif // THREAD_OUT_H
