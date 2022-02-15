#include "main_in.h"
#include "ui_main_in.h"
#include<Tool/package.h>
#include<Tool/singledb.h>
#include <QFileDialog>
#include<QFile>
#include <QTextStream>
#include <QMessageBox>
#include <Thread/thread.h>
#include "Sqlite/car_model.h"
#include <unistd.h>

int Main_In::mark=0;
int Main_In::car_num=0;
Main_In::Main_In(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Main_In)
{
    ui->setupUi(this);
    car_num = 200-Thread::getInstance()->get_car_amount();
    this->location = "西大门";
    QDateTime date_time =QDateTime::currentDateTime();
    this->date =date_time.toString("yyyy-MM-dd");
    cout<<this->date.toStdString().c_str()<<endl;
    //表头创建
    ui->tableWidget->setColumnCount(4);
    QStringList header;
    header<<"序号"<<"车牌号码"<<"时间"<<"位置";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->setColumnWidth(0,20);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,140);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui-> tableWidget->setAlternatingRowColors(true);
    //倒序插入数据
    ui-> tableWidget->sortByColumn(1, Qt::DescendingOrder);
    insertTableItems();
    //车牌识别参数初始化
    pr.setDetectType(PR_DETECT_CMSER | PR_DETECT_COLOR);
    pr.setResultShow(false);
    pr.setMaxPlates(4);
    //线程打开
    this->thread_in=new Thread_In();
    init_connect();
    this->thread_in->start();
}

void Main_In::init_connect()
{
    connect(thread_in, SIGNAL(sendImg(QImage)), this, SLOT(receiveimg(QImage)));
    connect(thread_in, SIGNAL(sendMat(Mat)), this, SLOT(receivemat(Mat)),Qt::BlockingQueuedConnection);
    connect(Thread::getInstance(),SIGNAL(photo_continue_in(int)),this,SLOT(photo_send(int)));
}

//加载当日的入场车辆信息
void Main_In::insertTableItems()
{
    int i,nCount;
    int row,col;
    char **qres;
    qres = Car_Model::getInstance()->get_in_car(date);
    row = Car_Model::getInstance()->get_row();
    col = Car_Model::getInstance()->get_col();

    //创建表格item，加载到表格中
    QTableWidgetItem *item[4];
    this->num = row;
    for (i=0;i<row;i++)
    {
        nCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(nCount);

        //序号
        item[0] = new QTableWidgetItem(QString("%1") .arg(num--));
        ui->tableWidget->setItem(i, 0, item[0]);

        //车牌号码
        item[1] = new QTableWidgetItem(QString(qres[(i+1) * col]));
        ui->tableWidget->setItem(i, 1, item[1]);

        //时间
        item[2] = new QTableWidgetItem(QString(qres[(i+1) * col +1]));
        ui->tableWidget->setItem(i, 2, item[2]);

        //位置
        item[3] = new QTableWidgetItem(QString(qres[(i+1) * col +2]));
        ui->tableWidget->setItem(i, 3, item[3]);
    }
    this->num = row+1;
}

void Main_In::receiveimg(QImage img)
{
    //实时更新图片
    this->Image =img;
    this->update();
}

void Main_In::receivemat(Mat mat)
{
    this->mat = mat.clone();
}

//label画面显示绘制事件
void Main_In::paintEvent(QPaintEvent *)
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


Main_In::~Main_In()
{
    delete ui;
}

//点击车牌识别槽函数
void Main_In::on_carNum_butt_clicked()
{
    mark = 1;
    //先清空，防止二次点击不识别新车牌
    plateVec.clear();
    pr.plateRecognize(mat, plateVec);
    if (plateVec.size()>0)
    {
        //车牌识别
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
        //字符串部分选择，舍弃颜色内容
        carInfo = QString::fromStdString(liences).mid(3,-1);
        this->old_car_date = carInfo;
        ui->carNum_edit->setText(carInfo);

        //控件显示入场时间
        date_time =QDateTime::currentDateTime();
        this->time = date_time.toString("yyyy-MM-dd hh:mm:ss");
        ui->inTime_edit->setText(time);

        /*图片存本地
        path = SingleDB::getInstance()->get_image_path()+"/"+time;
        car_image.save(path,"JPG",-1);

        QString path_in = SingleDB::getInstance()->get_image_path()+"/"+time+"_big";
        Image.save(path_in,"JPG",-1);
        queue_pathstr.push_back(path_in);


        //车牌信息存本地
        Car_Model::getInstance()->add_car(carInfo, date, time, location);
        char **qres2 = Car_Model::getInstance()->get_last_car_num();

        //在表的首部加入新的车辆信息
        QTableWidgetItem *item2[4];
        ui->tableWidget->insertRow(0);
        car_num++;
        emit update_car_amount(car_num);

        //序号
        item2[0] = new QTableWidgetItem(QString("%1") .arg(num++));
        ui->tableWidget->setItem(0, 0, item2[0]);

        //车牌号码
        item2[1] = new QTableWidgetItem(QString(qres2[3]));
        ui->tableWidget->setItem(0, 1, item2[1]);

        //时间
        item2[2] = new QTableWidgetItem(QString(qres2[4]));
        ui->tableWidget->setItem(0, 2, item2[2]);

        //位置
        item2[3] = new QTableWidgetItem(QString(qres2[5]));
        ui->tableWidget->setItem(0, 3, item2[3]);
//        for(int i=0;i<4;i++)
//        {
//            item2[i]->~QTableWidgetItem();
//        }

        //发到服务器
        PACK_HEAD head;
        PACK_ENTER car_in;
        char buffer[sizeof(PACK_HEAD) + sizeof(PACK_ENTER)] = { 0 };
        memset(&head, 0, sizeof(PACK_HEAD));
        memset(&car_in, 0, sizeof(PACK_ENTER));

        //包的类型是入场包
        head.bodyType = CAR_GETIN;
        //将标签控件里面内容赋值给结构体
        strcpy(car_in.car_num,ui->carNum_edit->text().toStdString().c_str());
        strcpy(car_in.now_time,ui->inTime_edit->text().toStdString().c_str());
        strcpy(car_in.location,location.toStdString().c_str());
        strcpy(car_in.photo_path,path.toStdString().c_str());

        //拷贝包头+包体，发送到服务器
        memcpy(buffer, &head, sizeof(PACK_HEAD));
        memcpy(buffer + sizeof(PACK_HEAD), &car_in, sizeof(PACK_ENTER));
        write(SingleDB::getInstance()->get_fd(), buffer, sizeof(PACK_HEAD) + sizeof(PACK_ENTER));



        //传图片到服务器
        usleep(2000);

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

        QString save_name = date_time.toString("yyyy-MM-dd_hh:mm:ss")+".jpg";
        strcpy(photo.filename,save_name.toStdString().c_str());
        photo.num=1;
        memcpy(buff,&head_photo,sizeof (PACK_HEAD));
        fseek(fp,0,SEEK_SET);
        photo.realSize=fread(photo.context, 1, 1024, fp);
        memcpy(buff+sizeof(PACK_HEAD),&photo,sizeof(PACK_PHOTO));
        this->queue_pathstr.pop_front();
        write(SingleDB::getInstance()->get_fd(), buff, sizeof(PACK_HEAD) + sizeof(PACK_PHOTO));*/
    }
}



//确定车牌识别
void Main_In::on_modifyNum_butt_clicked()
{
    if(ui->carNum_edit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("提示！"),tr("车牌识别为空!"),QMessageBox::Yes);
        return;
    }
    ui->modifyNum_butt->setEnabled(false);
    ui->carNum_butt->setEnabled(false);

    /*图片存本地*/
    path = SingleDB::getInstance()->get_image_path()+"/"+time;
    car_image.save(path,"JPG",-1);

    QString path_in = SingleDB::getInstance()->get_image_path()+"/"+time+"_big";
    Image.save(path_in,"JPG",-1);
    queue_pathstr.push_back(path_in);

    /*车牌信息存本地*/
    Car_Model::getInstance()->add_car(ui->carNum_edit->text(), date, time, location);
    char **qres2 = Car_Model::getInstance()->get_last_car_num();

    //在表的首部加入新的车辆信息
    QTableWidgetItem *item2[4];
    ui->tableWidget->insertRow(0);
    --car_num;
    emit update_car_amount(car_num);

    //序号
    item2[0] = new QTableWidgetItem(QString("%1") .arg(num++));
    ui->tableWidget->setItem(0, 0, item2[0]);

    //车牌号码
    item2[1] = new QTableWidgetItem(QString(qres2[3]));
    ui->tableWidget->setItem(0, 1, item2[1]);

    //时间
    item2[2] = new QTableWidgetItem(QString(qres2[4]));
    ui->tableWidget->setItem(0, 2, item2[2]);

    //位置
    item2[3] = new QTableWidgetItem(QString(qres2[5]));
    ui->tableWidget->setItem(0, 3, item2[3]);
//        for(int i=0;i<4;i++)
//        {
//            item2[i]->~QTableWidgetItem();
//        }

    /*发到服务器*/
    PACK_HEAD head;
    PACK_ENTER car_in;
    char buffer[sizeof(PACK_HEAD) + sizeof(PACK_ENTER)] = { 0 };
    memset(&head, 0, sizeof(PACK_HEAD));
    memset(&car_in, 0, sizeof(PACK_ENTER));

    //包的类型是入场包
    head.bodyType = CAR_GETIN;
    //将标签控件里面内容赋值给结构体
    strcpy(car_in.car_num,ui->carNum_edit->text().toStdString().c_str());
    strcpy(car_in.now_time,ui->inTime_edit->text().toStdString().c_str());
    strcpy(car_in.location,location.toStdString().c_str());
    strcpy(car_in.photo_path,path.toStdString().c_str());

    //拷贝包头+包体，发送到服务器
    memcpy(buffer, &head, sizeof(PACK_HEAD));
    memcpy(buffer + sizeof(PACK_HEAD), &car_in, sizeof(PACK_ENTER));
    write(SingleDB::getInstance()->get_fd(), buffer, sizeof(PACK_HEAD) + sizeof(PACK_ENTER));

    //传图片到服务器
    usleep(2000);

    char buff[sizeof(PACK_HEAD) + sizeof(PACK_PHOTO)] = {0};
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

    QString save_name = date_time.toString("yyyy-MM-dd_hh:mm:ss")+".jpg";
    strcpy(photo.filename,save_name.toStdString().c_str());
    photo.num=1;
    memcpy(buff,&head_photo,sizeof (PACK_HEAD));
    fseek(fp,0,SEEK_SET);
    photo.realSize=fread(photo.context, 1, 1024, fp);
    memcpy(buff+sizeof(PACK_HEAD),&photo,sizeof(PACK_PHOTO));
    this->queue_pathstr.pop_front();
    write(SingleDB::getInstance()->get_fd(), buff, sizeof(PACK_HEAD) + sizeof(PACK_PHOTO));
    //存到本地数据库
    Car_Model::getInstance()->change_car(this->old_car_date.toStdString().c_str(),ui->carNum_edit->text().toStdString().c_str());
}

void Main_In::photo_send(int result)
{
    if(mark ==1)
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
                mark =0;
                ui->modifyNum_butt->setEnabled(true);
                ui->carNum_butt->setEnabled(true);
                QMessageBox::warning(this, tr("提示！"),tr("图片发送成功！"),QMessageBox::Yes);
                ui->carNum_edit->setText("");
                ui->inTime_edit->setText("");
                return;
            }

            photo.num++;
            memcpy(buffer+sizeof(PACK_HEAD),&photo,sizeof(PACK_PHOTO));
            write(SingleDB::getInstance()->get_fd(), buffer, sizeof(PACK_HEAD) + sizeof(PACK_PHOTO));
            cout<<"2"<<endl;
        }
        else
        {
            fclose(fp);
        }
    }
}
