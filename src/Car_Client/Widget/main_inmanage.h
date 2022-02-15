#ifndef MAIN_INMANAGE_H
#define MAIN_INMANAGE_H
#include <QWidget>
#include <unistd.h>
//#include <opencv2/opencv.hpp>
#include"Thread/thread_inmanage.h"
#include<Thread/thread_in.h>
#include <QTimer>

#include"Tool/package.h"
#include "Thread/thread.h"
using namespace cv;
using namespace easypr;
#define MANUAL 1 //手动录制
#define AUTO   2 //自动录制
#define STOP   0 //无录制
namespace Ui {
class Main_Inmanage;
}

class Main_Inmanage : public QWidget
{
    Q_OBJECT
public:
    explicit Main_Inmanage(QWidget *parent = nullptr);
    ~Main_Inmanage();
    void init_connect();
    Thread_Inmanage *thread_inmanage;
    void update_car();

private slots:
    void on_record_butt_clicked();
    void receiveimg(QImage img);


    void on_auto_butt_toggled(bool checked);
    void butt_enabel();

    void on_snap_butt_clicked();
    void photo_send(int result);
    void receivemat(Mat mat);




private:
    Ui::Main_Inmanage *ui;

    CPlateRecognize pr;
    vector <CPlate> plateVec;
    CPlate plate;
    Mat plateMat;
    CascadeClassifier cascade;
    Mat plateMat_car;
    list<QString> queue_pathstr;
    QImage car_image;
    VideoCapture cap;
    VideoWriter write;
    QString time;
    static int mark;
    int counter;  //记录帧数
    int state;

    Mat frame;
    Mat mat;
    QImage Image;

    QPixmap pixmap;
    QPixmap fitpixmap;

    FILE*fp;
    PACK_PHOTO photo;
    PACK_HEAD head_photo;



protected:

    void paintEvent(QPaintEvent*);


};

#endif // MAIN_INMANAGE_H
