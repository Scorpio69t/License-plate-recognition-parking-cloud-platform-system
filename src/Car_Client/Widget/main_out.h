#ifndef MAIN_OUT_H
#define MAIN_OUT_H

#include <QWidget>
#include "Thread/thread_out.h"
#include<Thread/thread_in.h>
#include <QPainter>
#include <QEvent>
#include <QDateTime>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include<Thread/thread.h>
using namespace cv;

namespace Ui {
class Main_Out;
}

class Main_Out : public QWidget
{
    Q_OBJECT

public:
    explicit Main_Out(QWidget *parent = nullptr);
    ~Main_Out();
    void init_connect();



private:
    Ui::Main_Out *ui;

    Thread_Out *thread_out;
    QImage Image, car_image;
    Mat mat;

    Mat plateMat_car;


    CPlateRecognize pr;
    vector <CPlate> plateVec;
    CPlate plate;
    Mat plateMat;

    QPixmap pixmap;
    QPixmap fitpixmap;

    QDateTime date_time;



protected:
    void paintEvent(QPaintEvent*);
private slots:
    void on_carNum_butt_clicked();
    void receiveimg(QImage img);
    void receivemat(Mat mat);
    void out_msg(PACK_EXIT_BACK msg);
    void on_out_butt_clicked();

signals:
    void update_car_amount(int num);
};

#endif // MAIN_OUT_H
