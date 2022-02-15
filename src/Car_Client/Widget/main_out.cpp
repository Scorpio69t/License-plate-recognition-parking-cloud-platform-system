#include "main_out.h"
#include "ui_main_out.h"
#include <QDateTime>
#include<Tool/singledb.h>
#include<Widget/main_in.h>
#include <QMetaType>

Main_Out::Main_Out(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Main_Out)
{
    qRegisterMetaType<PACK_EXIT_BACK>("PACK_EXIT_BACK");
    ui->setupUi(this);
    pr.setDetectType(PR_DETECT_CMSER | PR_DETECT_COLOR);
    pr.setResultShow(false);
    pr.setMaxPlates(4);

    //线程打开
    this->thread_out=new Thread_Out();
    init_connect();
    this->thread_out->start();
}

Main_Out::~Main_Out()
{
    delete ui;
}

void Main_Out::init_connect()
{
    connect(thread_out, SIGNAL(sendImg(QImage)), this, SLOT(receiveimg(QImage)));
    connect(thread_out, SIGNAL(sendMat(Mat)), this, SLOT(receivemat(Mat)),Qt::BlockingQueuedConnection);
    connect(Thread::getInstance(),SIGNAL(out(PACK_EXIT_BACK)),this,SLOT(out_msg(PACK_EXIT_BACK)));

}

void Main_Out::paintEvent(QPaintEvent *)
{
    if(!this->Image.isNull())
    {

        //在这里加入ui->label更新
        pixmap = QPixmap::fromImage(Image);
        fitpixmap = pixmap.scaled(ui->outVideo_label->width(), ui->outVideo_label->height(),
                                          Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        //控件接收图片
        ui->outVideo_label->setPixmap(fitpixmap);
        ui->outVideo_label->setScaledContents(true);
    }

}

void Main_Out::on_carNum_butt_clicked()
{

    plateVec.clear();
    pr.plateRecognize(mat, plateVec);
    if (plateVec.size()>0)
    {
        plate = plateVec.at(0);
        plateMat = plate.getPlateMat();
        cvtColor (plateMat, plateMat_car, CV_BGR2RGB);
        car_image = QImage(plateMat_car.data, plateMat_car.cols, plateMat_car.rows, QImage::Format_RGB888);

        //控件显示车牌图片
        QPixmap *image = new QPixmap(QPixmap::fromImage(car_image));
        image->scaled(ui->carNum_image->size(),Qt::KeepAspectRatio);
        ui->carNum_image->setPixmap(QPixmap::fromImage(car_image));

        //控件显示车牌号
        string liences=plate.getPlateStr();
        QString carInfo = QString::fromStdString(liences).mid(3,-1);;
        ui->carNum_edit->setText(carInfo);

        //控件显示出场时间
        date_time =QDateTime::currentDateTime();
        QString current_date =date_time.toString("yyyy-MM-dd hh:mm:ss");
        ui->outTime_edit->setText(current_date);

        //图片存本地
        QString path = SingleDB::getInstance()->get_image_path()+"/"+current_date;
        car_image.save(path,"JPG",-1);

        //发到服务器
        PACK_HEAD head;
        PACK_EXIT car_out;
        char buffer[sizeof(PACK_HEAD) + sizeof(PACK_EXIT)] = { 0 };
        memset(&head, 0, sizeof(PACK_HEAD));
        memset(&car_out, 0, sizeof(PACK_EXIT));

        head.bodyType=CAR_GETOUT;
        //将标签控件里面内容赋值给结构体
        strcpy(car_out.car_number,ui->carNum_edit->text().toStdString().c_str());
        strcpy(car_out.out_time,ui->outTime_edit->text().toStdString().c_str());

        //拷贝包体+包体
        memcpy(buffer, &head, sizeof(PACK_HEAD));
        memcpy(buffer + sizeof(PACK_HEAD), &car_out, sizeof(PACK_EXIT));
        write(SingleDB::getInstance()->get_fd(), buffer, sizeof(PACK_HEAD) + sizeof(PACK_EXIT));
    }
}





void Main_Out::receiveimg(QImage img)
{
    this->Image =img;
    this->update();
}

void Main_Out::receivemat(Mat mat)
{
    this->mat = mat.clone();
}

void Main_Out::out_msg(PACK_EXIT_BACK msg)
{
    ui->inTime_edit->setText(msg.in_time);
    ui->stayTime_edit->setText(msg.total_time);//
    ui->price_label->setText("收费："+QString::number(msg.money)+"元");
    if(msg.vip)
    {
        ui->radioButton->setChecked(1);
    }
    else
    {
        ui->radioButton->setChecked(0);
    }
}

void Main_Out::on_out_butt_clicked()
{

    Main_In::car_num++;
    emit update_car_amount(Main_In::car_num);
}
