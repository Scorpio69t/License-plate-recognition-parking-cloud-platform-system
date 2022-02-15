#ifndef MAIN_IN_H
#define MAIN_IN_H

#include <QWidget>
#include <QPainter>
#include <QEvent>
#include <QDateTime>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include<Thread/thread_in.h>
#include "Thread/thread.h"
using namespace cv;
using namespace easypr;
namespace Ui {
class Main_In;
}

class Main_In : public QWidget
{
    Q_OBJECT

public:
    explicit Main_In(QWidget *parent = nullptr);
    ~Main_In();
    void init_connect();
    void get_image_path();
    void insertTableItems();
    static int car_num;
    static int mark;



private:
    Ui::Main_In *ui;
    Thread_In *thread_in;
    QImage Image, car_image;
    Mat mat,mat_RGB;

    Mat plateMat_car;

    CPlateRecognize pr;
    vector <CPlate> plateVec;
    CPlate plate;
    Mat plateMat;

    QString location;
    QString old_car_date;
    QString date;
    QString time;

    QPixmap pixmap;
    QPixmap fitpixmap;

    list<QString> queue_pathstr;

    FILE*fp;
    PACK_PHOTO photo;
    int num;
    PACK_HEAD head_photo;

    QString path;
    QString carInfo;
    QDateTime date_time;


private slots:
    void receiveimg(QImage img);
    void receivemat(Mat mat);
    void on_carNum_butt_clicked();

    void on_modifyNum_butt_clicked();

    void photo_send(int result);

signals:
    void update_car_amount(int num);
protected:
    void paintEvent(QPaintEvent*);
};

#endif // MAIN_IN_H
