#ifndef BEGINWIDGET_H
#define BEGINWIDGET_H
#include <QBrush>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QIcon>
#include "Tool/myitem.h" //自己定义的图元类
#include <QPixmap>
#include<QTimer>
#include<Widget/login.h>
#include<Widget/set.h>
class BeginWidget:public QGraphicsView
{
    Q_OBJECT
public:
    BeginWidget();

    Login *loginwin;
    Set *setwin;
    QGraphicsScene *scene;
    MyItem *item1,*item2;
    QTimer *timer;
    void init_UI();
    void init_control();
    void init_connect();
public slots:
    void watchStatus();
    void toSettingorMainWidget();


};

#endif // BEGINWIDGET_H
