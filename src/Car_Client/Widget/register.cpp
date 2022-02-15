#include "register.h"
#include "ui_register.h"
#include "Tool/package.h"
#include "Thread/thread.h"
#include <QMessageBox>
Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    this->mark = 0;
    this->mark2 = 0;
    ui->pwd_edit->setEchoMode(QLineEdit::Password);
    ui->pwd_edit_2->setEchoMode(QLineEdit::Password);
    init_connect();
}

Register::~Register()
{
    delete ui;
}

void Register::init_connect()
{
    connect(ui->esc_butt,SIGNAL(clicked()),this,SIGNAL(back()));
    connect(Thread::getInstance(),SIGNAL(Register(bool)),this,SLOT(doregister(bool)));
}

void Register::on_eye_clicked()
{
    //0是密文 1是明文
        if(this->mark==0)
        {
            ui->pwd_edit->setEchoMode(QLineEdit::Normal);
            this->mark=1;
            cout<<"1"<<endl;
        }
        else if(this->mark==1)
        {
            ui->pwd_edit->setEchoMode(QLineEdit::Password);
            this->mark=0;
            cout<<"0"<<endl;
        }
}

void Register::on_eye2_clicked()
{
    //0是密文 1是明文
        if(this->mark2==0)
        {
            ui->pwd_edit_2->setEchoMode(QLineEdit::Normal);
            this->mark2=1;
        }
        else if(this->mark2==1)
        {
            ui->pwd_edit_2->setEchoMode(QLineEdit::Password);
            this->mark2=0;
        }
}

void Register::on_register_butt_clicked()
{
    //如果内容为空就提示
    if(ui->name_edit->text().trimmed() == tr("") || ui->name_edit->text() == tr(""))
    {
        QMessageBox::warning(this, tr("提示！"),tr("内容不能为空！"),QMessageBox::Yes);
    }
    else
    {
        //如果长度大于5就继续判断
        if(ui->name_edit->text().length()>5 && ui->pwd_edit->text().length()>5)
        {
            //如果密码和二次输入的密码一致就继续判断
            if(ui->pwd_edit->text() == ui->pwd_edit_2->text())
            {
                PACK_HEAD head;
                PACK_REGIST_LOGIN login_body;
                char buffer[sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN)] = { 0 };
                memset(&head, 0, sizeof(PACK_HEAD));
                memset(&login_body, 0, sizeof(PACK_REGIST_LOGIN));
                //包头类型为注册
                head.bodyType=1;
                //将编辑框里面内容赋值给结构体
                strcpy(login_body.name,ui->name_edit->text().toStdString().c_str());
                strcpy(login_body.pwd,ui->pwd_edit->text().toStdString().c_str());
                //拷贝包体+包体
                memcpy(buffer, &head, sizeof(PACK_HEAD));
                memcpy(buffer + sizeof(PACK_HEAD), &login_body, sizeof(PACK_REGIST_LOGIN));
                write(SingleDB::getInstance()->get_fd(), buffer, sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN));
            }
        }
    }
    // 清空输入框内容
    ui->name_edit->clear();
    ui->pwd_edit->clear();
    ui->pwd_edit_2->clear();

}

void Register::doregister(bool success)
{
    if(success == true)
    {
        QMessageBox::warning(this, tr("提示！"),tr("注册成功！"),QMessageBox::Yes);
        back();
    }
    else
    {
        QMessageBox::warning(this, tr("提示！"),tr("已经有该用户！"),QMessageBox::Yes);
    }
}


