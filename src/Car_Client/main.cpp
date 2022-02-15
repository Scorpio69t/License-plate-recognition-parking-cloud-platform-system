#include "Widget/login.h"
#include <QApplication>
#include "Widget/main_interface.h"
#include <iostream>
#include "Widget/beginwidget.h"
#include "Widget/main_replay.h"
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BeginWidget w;
    w.show();
    return a.exec();
}
