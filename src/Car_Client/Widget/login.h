#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Tool/singledb.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QMessageBox>
#include <QDialog>
#include "Widget/register.h"
#include "main_interface.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    //连接函数
    void init_connect();

    //生成验证码函数
    QString getcode();
    //验证码干扰元素函数
    void paintEvent(QPaintEvent *event);
    //重写鼠标点击事件，没点击一次就随机生成新的验证码
    void mousePressEvent(QMouseEvent *event);


    static QString user;


private slots:
    void on_login_butt_clicked();

    void on_register_butt_clicked();

    void on_eye_clicked();

    void backLogin();

    void dologin(bool success);

    void tomainwin();



private:
    Ui::Login *ui;
    QString code;

    Register* regWin;
    int mark;
    Main_Interface *mainwin;

signals:
   void tomain();

};

#endif // WIDGET_H
