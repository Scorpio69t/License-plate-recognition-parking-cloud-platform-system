#include "main_interface.h"
#include "ui_main_interface.h"
#include "Widget/login.h"
#include <QDateTime>
#include <Thread/thread.h>
Main_Interface::Main_Interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Main_Interface)
{
    this->mark = 0;
    ui->setupUi(this);
    ui->surplus_label->setText(QString("剩余车位：")+QString::number(200-Thread::getInstance()->get_car_amount()));
    ui->user_label->setText(QString("当前用户：")+Login::user);
    //将入场模式加入堆栈布局
    this->inwin = new Main_In();
    ui->stackedWidget->addWidget(inwin);
    //将出场模式加入堆栈布局
    this->outwin = new Main_Out();
    ui->stackedWidget->addWidget(outwin);
    //将停车场内部加入堆栈布局
    this->inmanagewin = new Main_Inmanage();
    ui->stackedWidget->addWidget(inmanagewin);


    this->equerywin = new Main_Vehicl_Equery();
    ui->stackedWidget->addWidget(equerywin);


     this->playbackwin = new Main_Playback();
     ui->stackedWidget->addWidget(playbackwin);

    //设置当前布局为入模式
    ui->stackedWidget->setCurrentWidget(inwin);
    //定时器--刷新时间
    this->time =new QTimer(this);
    this->time->start(500);
    init_connect();
}

Main_Interface::~Main_Interface()
{
    delete ui;
}

void Main_Interface::init_connect()
{
    connect(this->time, SIGNAL(timeout()), this, SLOT(qtimeSlot()));
    connect(this->inwin, SIGNAL(update_car_amount(int)), this, SLOT(update_car_amount(int)));
    connect(this->outwin, SIGNAL(update_car_amount(int)), this, SLOT(update_car_amount(int)));
}



//获取当前时间函数
void Main_Interface::qtimeSlot()
{

    QDateTime date_time =QDateTime::currentDateTime();
    QString current_date =date_time.toString("当前系统时间：yyyy-MM-dd hh:mm:ss");
    ui->time_label->setText(current_date);

}

void Main_Interface::on_out_in_butt_clicked()
{
    if(this->mark == 0)
    {
        ui->stackedWidget->setCurrentWidget(outwin);
        ui->out_in_butt->setText("进场模式");
        this->mark = 1;this->mark = 1;
    }
    else
    {
        ui->stackedWidget->setCurrentWidget(inwin);
        ui->out_in_butt->setText("出场模式");
        this->mark = 0;
    }
}

void Main_Interface::on_inside_butt_clicked()
{
    ui->stackedWidget->setCurrentWidget(inmanagewin);
    inmanagewin->update_car();
}

void Main_Interface::on_set_butt_clicked()
{
    this->setwin = new Set();
    setwin->show();
}

void Main_Interface::on_query_butt_clicked()
{
    ui->stackedWidget->setCurrentWidget(equerywin);
}

void Main_Interface::on_playback_butt_clicked()
{
    ui->stackedWidget->setCurrentWidget(playbackwin);
}

void Main_Interface::update_car_amount(int num)
{
    ui->surplus_label->setText(QString("剩余车位：")+QString::number(num));
}
