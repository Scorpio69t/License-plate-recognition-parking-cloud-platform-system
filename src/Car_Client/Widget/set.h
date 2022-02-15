#ifndef SET_H
#define SET_H

#include <QWidget>

#include <Tool/singledb.h>
namespace Ui {
class Set;
}

class Set : public QWidget
{
    Q_OBJECT

public:
    explicit Set(QWidget *parent = nullptr);
    ~Set();
    void init_connect();

private slots:
    void on_OK_butt_clicked();
   void Select_VideoPath();
   void Select_ImagePath();

private:
    Ui::Set *ui;
};

#endif // SET_H
