#include "main_vehicl_equery.h"
#include "ui_main_vehicl_equery.h"
#include<QMessageBox>
#include<Tool/singledb.h>
#include <QMetaType>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
Main_Vehicl_Equery::Main_Vehicl_Equery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Main_Vehicl_Equery)
{
    ui->setupUi(this);
    qRegisterMetaType<PACK_ALLCARMSG_BACK>("PACK_ALLCARMSG_BACK");
    num =1;
    ui->tableWidget->setColumnCount(7) ;
    QStringList header ;
    header<<"序号"<<"车牌号码"<<"入场时间"<<"出场时间"<<"入场图片"<<"出场图片"<<"停车金额";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->setColumnWidth(0,20);
    ui->tableWidget->setColumnWidth(1,170);
    ui->tableWidget->setColumnWidth(2,170);
    ui->tableWidget->setColumnWidth(3,170);
    ui->tableWidget->setColumnWidth(4,170);
    ui->tableWidget->setColumnWidth(5,170);


    //倒序插入数据
    ui->tableWidget->sortByColumn(1, Qt::DescendingOrder);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui-> tableWidget->setAlternatingRowColors(true);
    init_connect();
}

Main_Vehicl_Equery::~Main_Vehicl_Equery()
{
    delete ui;
}

void Main_Vehicl_Equery::init_connect()
{
    connect(Thread::getInstance(),SIGNAL(car_msg(PACK_ALLCARMSG_BACK)),this,SLOT(show_msg(PACK_ALLCARMSG_BACK)));
}

void Main_Vehicl_Equery::send_msg()
{
    /*发到服务器*/
    PACK_HEAD head;
    PACK_CARMSG car_equery;
    char buffer[sizeof(PACK_HEAD) + sizeof(PACK_CARMSG)] = { 0 };
    memset(&head, 0, sizeof(PACK_HEAD));
    memset(&car_equery, 0, sizeof(PACK_CARMSG));

    //包体类型是车辆信息包
    head.bodyType=CAR_MSG_TYPE;
    //将标签控件里面内容赋值给结构体
    strcpy(car_equery.car_number,ui->carNum_edit->text().toStdString().c_str());
    strcpy(car_equery.in_time,ui->inTime_edit->text().toStdString().c_str());
    strcpy(car_equery.out_time,ui->outTime_edit->text().toStdString().c_str());
    car_equery.page = frequency;
    //拷贝包头+包体
    memcpy(buffer, &head, sizeof(PACK_HEAD));
    memcpy(buffer + sizeof(PACK_HEAD), &car_equery, sizeof(PACK_CARMSG));
    write(SingleDB::getInstance()->get_fd(), buffer, sizeof(PACK_HEAD) + sizeof(PACK_CARMSG));
}

void Main_Vehicl_Equery::on_equery_label_clicked()
{
    ui->tableWidget->setRowCount(0);
    num =1;
    frequency = 0;
    send_msg();

}

void Main_Vehicl_Equery::on_more_butt_clicked()
{
    frequency++;
    send_msg();
}

void Main_Vehicl_Equery::show_msg(PACK_ALLCARMSG_BACK result)
{
    //如果没有结果就提示并返回
    if(result.realCount==0)
    {
        QMessageBox::warning(this, tr("提示！"),tr("没有内容！"),QMessageBox::Yes);
        return;
    }
    QTableWidgetItem *item[5];
    for(int i = 0;i<result.realCount;i++)
    {
        ui->tableWidget->insertRow(0);
        ui->tableWidget->setRowHeight(0,30);

        //序号  "序号"<<"车牌号码"<<"入场时间"<<"出场时间"<<"入场图片"<<"出场图片"<<"停车金额";
        item[0] = new QTableWidgetItem(QString("%1") .arg(num++));

        ui->tableWidget->setItem(0, 0, item[0]);

        //车牌号码
        item[1] = new QTableWidgetItem(QString(result.arr[i].car_number));
        ui->tableWidget->setItem(0, 1, item[1]);

        //入场时间
        item[2] = new QTableWidgetItem(QString(result.arr[i].in_time));
        ui->tableWidget->setItem(0, 2, item[2]);

        //出场时间
        item[3] = new QTableWidgetItem(QString(result.arr[i].out_time));
        ui->tableWidget->setItem(0, 3, item[3]);

        //入场图片
        QLabel *in_label = new QLabel();
        in_label->setPixmap(QPixmap(SingleDB::getInstance()->get_image_path()+"/"+result.arr->in_time));
        ui->tableWidget->setCellWidget(0, 4, in_label);

        //出场图片
        QLabel *out_label = new QLabel();
        out_label->setPixmap(QPixmap(SingleDB::getInstance()->get_image_path()+"/"+result.arr->out_time));
        ui->tableWidget->setCellWidget(0, 5, out_label);

        //出场时间
        item[4] = new QTableWidgetItem(QString::number(result.arr[i].money));
        ui->tableWidget->setItem(0, 6, item[4]);

    }

}

void Main_Vehicl_Equery::on_export_butt_clicked()
{
    QDateTime date_time =QDateTime::currentDateTime();
    QString date =date_time.toString("yyyy-MM-dd hh:mm:ss");

    QFile file(QString("./data/")+date+QString("_date.txt"));
    //已读写方式打开文件，
    //如果文件不存在会自动创建文件
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("提示！"),tr("导出失败！"),QMessageBox::Yes);
        return;
    }
    else
    {
        QTextStream in(&file);
        cout<<ui->tableWidget->rowCount();
        cout<<ui->tableWidget->columnCount();
        for(int i=0 ;i<ui->tableWidget->rowCount();i++)
        {
            for(int j=0;j<7;j++)
            {
                if(j == 5 ||j == 4)
                {
                    continue;
                }
                in<<ui->tableWidget->item(i,j)->text()<<" ";
            }
            in<<"\n";
        }
        file.close();
    }
    QMessageBox::warning(this, tr("提示！"),tr("导出成功！"),QMessageBox::Yes);
}
