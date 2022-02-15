#include "login.h"
#include "ui_login.h"
#include "Tool/package.h"
#include "Thread/thread.h"

QString Login::user;
Login::Login(QWidget *parent):QWidget(parent),ui(new Ui::Login)
{
    ui->setupUi(this);
    this->mark = 0;
    this->code = this->getcode();
    //密文显示
    ui->pwd_edit->setEchoMode(QLineEdit::Password);
    this->regWin = new Register();

    init_connect();
    this->update();

}

Login::~Login()
{
    delete ui;
}

void Login::init_connect()
{

    connect(this->regWin,SIGNAL(back()),this,SLOT(backLogin()));
    connect(ui->eye,SIGNAL(clicked()),this,SLOT(on_eye_clicked()));
    connect(Thread::getInstance(),SIGNAL(Login(bool)),this,SLOT(dologin(bool)));
    connect(this,SIGNAL(tomain()),this,SLOT(tomainwin()));

}


//生成验证码函数
QString Login::getcode()
{
    update();
    //code为QString类型变量
    QString getcode;
    //随机数字
    for(int i = 0;i < 4;++i)
    {
        int num = qrand()%3;
        if(num == 0)
        {
            //数字
            getcode += QString::number(qrand()%10);
        }
        else if(num == 1)
        {
            //大写字母
            int temp = 'A';
            getcode += static_cast<QChar>(temp + qrand()%26);
        } else if(num == 2)
        {
            //小写字母
            int temp = 'a';
            getcode += static_cast<QChar>(temp + qrand()%26);
        }
    }
    return getcode;
}

//验证码干扰元素函数
void Login::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen;
    painter.drawPixmap(this->rect(),QPixmap(":image/pt.jpg"));

    //画点
    for(int i = 0;i < 100;++i)
    {
        pen = QPen(QColor(qrand()%256,qrand()%256,qrand()%256));
        painter.setPen(pen);
        painter.drawPoint(310+qrand()%150,170+qrand()%50);
    }
    //画线
    for(int i = 0;i < 10;++i)
    {

        painter.drawLine(310+qrand()%150,170+qrand()%50,
                         310+qrand()%150,170+qrand()%50);
    }
    pen = QPen(QColor(255,0,0,100));
    QFont font("楷体",25,QFont::Bold,true);
    painter.setFont(font);
    painter.setPen(pen);
    //绘画字
    for(int i = 0;i < 4;++i)
    {
        painter.drawText(310+30*i,170,30,40,Qt::AlignCenter,QString(code[i]));
    }
}

//重写鼠标点击事件，没点击一次就随机生成新的验证码
void Login::mousePressEvent(QMouseEvent *event)
{
    //验证码区域做重写鼠标点击事件
    int x = event->x();
    int y = event->y();
    if(x>310 && x<460 && y>170 && y<220)
    {
        this->code = this->getcode();
    }
}

//登录验证函数
void Login::on_login_butt_clicked()
{
    QString captcha = ui->code_edit->text();
    //
    if(captcha.toLower() == this->code.toLower())
    {
        PACK_HEAD head;
        PACK_REGIST_LOGIN login_body;
        char buffer[sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN)] = { 0 };
        memset(&head, 0, sizeof(PACK_HEAD));
        memset(&login_body, 0, sizeof(PACK_REGIST_LOGIN));
        //
        head.bodyType=2;
        //将编辑框里面内容赋值给结构体
        strcpy(login_body.name,ui->name_edit->text().toStdString().c_str());
        strcpy(login_body.pwd,ui->pwd_edit->text().toStdString().c_str());

        //拷贝包体+包体
        memcpy(buffer, &head, sizeof(PACK_HEAD));
        memcpy(buffer + sizeof(PACK_HEAD), &login_body, sizeof(PACK_REGIST_LOGIN));
        write(SingleDB::getInstance()->get_fd(), buffer, sizeof(PACK_HEAD) + sizeof(PACK_REGIST_LOGIN));
        return;
    }
    else
    {
        QMessageBox::warning(this, tr("警告！"),tr("验证码错误！"),QMessageBox::Yes);
        this->code = this->getcode();
    }
}

//根据服务器返回的布尔类型判断是否登陆成功
void Login::dologin(bool success)
{
    if(success == true)
    {
        this->user = ui->name_edit->text();
        emit tomain();
    }
    else
    {
        QMessageBox::warning(this, tr("提示！"),tr("帐号或密码错误！"),QMessageBox::Yes);
        // 清空输入框内容
        ui->name_edit->clear();
        ui->pwd_edit->clear();
        ui->code_edit->clear();
        this->update();
    }
}

void Login::tomainwin()
{
    this->hide();
    this->mainwin = new Main_Interface();
    this->mainwin->show();
}

//明文密文显示函数
void Login::on_eye_clicked()
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


    }
}

//跳转到注册
void Login::on_register_butt_clicked()
{
    this->hide();

    this->regWin->show();
    // 清空输入框内容
    ui->name_edit->clear();
    ui->pwd_edit->clear();
    ui->code_edit->clear();
}

//返回到登录
void Login::backLogin()
{
    this->regWin->close();
    this->show();
}


