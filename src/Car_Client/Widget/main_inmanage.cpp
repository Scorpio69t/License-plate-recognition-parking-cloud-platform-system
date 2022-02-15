#include "main_inmanage.h"
#include "ui_main_inmanage.h"
#include <Tool/singledb.h>
#include <unistd.h>
#include<QMessageBox>
//#include<Widget/main_out.h>

#include<QDateTime>
#include<Widget/main_in.h>
#include <sys/stat.h>

int Main_Inmanage::mark=0;
Main_Inmanage::Main_Inmanage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Main_Inmanage)
{
    ui->setupUi(this);
    ui->carTotal_label->setText(QString("目前车场总车辆：")+QString::number(Main_In::car_num));
    int freeParking = 200-Main_In::car_num;
    ui->freeParking_label->setText(QString("目前车场总车辆：")+QString::number(freeParking));
    counter = 0;
    pr.setDetectType(PR_DETECT_CMSER | PR_DETECT_COLOR) ;
    pr.setResultShow(false);
    pr.setMaxPlates(4);
    this->cascade.load("./data/cars.xml");
    //线程打开
    this->thread_inmanage=new Thread_Inmanage();
    init_connect();
    this->thread_inmanage->start();
}

Main_Inmanage::~Main_Inmanage()
{
    delete ui;
}


void Main_Inmanage::init_connect()
{

    connect(thread_inmanage, SIGNAL(sendImg(QImage)), this, SLOT(receiveimg(QImage)));
    connect(thread_inmanage, SIGNAL(butt_enable()), this, SLOT(butt_enabel()));
    connect(Thread::getInstance(),SIGNAL(photo_continue_in(int)),this,SLOT(photo_send(int)));
    connect(thread_inmanage, SIGNAL(sendMat(Mat)), this, SLOT(receivemat(Mat)),Qt::BlockingQueuedConnection);
}

void Main_Inmanage::on_record_butt_clicked()
{
    ui->record_butt->setEnabled(false);
    ui->auto_butt->setEnabled(false);
    thread_inmanage->video_save();
    thread_inmanage->state = MANUAL;

}

void Main_Inmanage::on_auto_butt_toggled(bool checked)
{
    if(checked == true)
    {
        thread_inmanage->state = AUTO;
    }
    else
    {
        thread_inmanage->state = STOP;
    }
}

void Main_Inmanage::butt_enabel()
{
    ui->record_butt->setEnabled(true);
    ui->auto_butt->setEnabled(true);
}




void Main_Inmanage::paintEvent(QPaintEvent *)
{
    if(!this->Image.isNull())
    {

        //在这里加入ui->label更新
        pixmap = QPixmap::fromImage(Image);
        fitpixmap = pixmap.scaled(ui->video_label->width(), ui->video_label->height(),
                                          Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        //控件接收图片
        ui->video_label->setPixmap(fitpixmap);
        ui->video_label->setScaledContents(true);
    }

}

void Main_Inmanage::receiveimg(QImage img)
{
    this->Image =img;
    this->update();
}

void Main_Inmanage::receivemat(Mat mat)
{
    this->mat = mat.clone();
}

void Main_Inmanage::on_snap_butt_clicked()
{
    mark =1;
    plateVec.clear();
    pr.plateRecognize(mat, plateVec);
    if (plateVec.size()>0)
    {
        plate = plateVec.at(0);
        plateMat = plate.getPlateMat();
        cvtColor (plateMat, plateMat_car, CV_BGR2RGB);
        car_image = QImage(plateMat_car.data, plateMat_car.cols, plateMat_car.rows, QImage::Format_RGB888);


        QDateTime date_time =QDateTime::currentDateTime();
        this->time = date_time.toString("yyyy-MM-dd hh:mm:ss");

        /*图片存本地*/
        QString path_in = SingleDB::getInstance()->get_image_path()+"/"+time+"_big2";
        Image.save(path_in,"JPG",-1);
        queue_pathstr.push_back(path_in);
        //传图片到服务器
        usleep(20000);

        char buff[sizeof(PACK_HEAD) + sizeof(PACK_PHOTO)] = { 0 };
        head_photo.bodyType = PHOTO_TYPE;

        if(this->queue_pathstr.empty())
        {
            return;
        }
        fp=fopen(this->queue_pathstr.front().toStdString().c_str(),"rb");
        fseek(fp, 0, SEEK_END);
        double size=ftell(fp);
        size=size/1024;
        photo.sum=ceil(size);
        QDateTime time = QDateTime::currentDateTime();

        QString save_name = date_time.toString("yyyy_MM_dd_hh_mm_ss")+".jpg";
        strcpy(photo.filename,save_name.toStdString().c_str());

        photo.num=1;
        memcpy(buff,&head_photo,sizeof (PACK_HEAD));
        fseek(fp,0,SEEK_SET);
        photo.realSize=fread(photo.context, 1, 1024, fp);
        memcpy(buff+sizeof(PACK_HEAD),&photo,sizeof(PACK_PHOTO));
        this->queue_pathstr.pop_front();
        ::write(SingleDB::getInstance()->get_fd(), buff, sizeof(PACK_HEAD) + sizeof(PACK_PHOTO));


    }
}


void Main_Inmanage::photo_send(int result)
{
    if(mark==1)
    {
        if(result == 1)
        {
            char buffer[sizeof(PACK_HEAD) + sizeof(PACK_PHOTO)] = { 0 };
            memcpy(buffer,&head_photo,sizeof (PACK_HEAD));
            photo.realSize=fread(photo.context, 1, 1024, fp);
            cout<<"done2"<<endl;
            if (photo.realSize == 0)
            {
                fclose(fp);
                mark=0;
                QMessageBox::warning(this, tr("提示！"),tr("图片发送成功！"),QMessageBox::Yes);
                return;
            }

            photo.num++;
            memcpy(buffer+sizeof(PACK_HEAD),&photo,sizeof(PACK_PHOTO));
            ::write(SingleDB::getInstance()->get_fd(), buffer, sizeof(PACK_HEAD) + sizeof(PACK_PHOTO));

            cout<<"2"<<endl;
        }
        else
        {
            fclose(fp);
        }
    }
}

void Main_Inmanage::update_car()
{
    ui->carTotal_label->setText(QString("目前车场总车辆：")+QString::number(Main_In::car_num));
    int freeParking = 200-Main_In::car_num;
    ui->freeParking_label->setText(QString("目前车场总车辆：")+QString::number(freeParking));
}
