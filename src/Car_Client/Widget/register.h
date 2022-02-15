#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include "Tool/singledb.h"


namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();
    void init_connect();

private slots:
    void on_eye_clicked();

    void on_eye2_clicked();

    void on_register_butt_clicked();

    void doregister(bool success);

private:
    Ui::Register *ui;
    int mark,mark2;

signals:
   void back();
};

#endif // REGISTER_H
