#ifndef MAIN_VEHICL_EQUERY_H
#define MAIN_VEHICL_EQUERY_H

#include <QWidget>
#include<Thread/thread.h>
namespace Ui {
class Main_Vehicl_Equery;
}

class Main_Vehicl_Equery : public QWidget
{
    Q_OBJECT

public:
    explicit Main_Vehicl_Equery(QWidget *parent = nullptr);
    ~Main_Vehicl_Equery();
    void init_connect();
    void send_msg();
private slots:
    void on_equery_label_clicked();

    void on_more_butt_clicked();

    void show_msg(PACK_ALLCARMSG_BACK result);

    void on_export_butt_clicked();

private:
    Ui::Main_Vehicl_Equery *ui;
    int frequency;
    int num;
};

#endif // MAIN_VEHICL_EQUERY_H
