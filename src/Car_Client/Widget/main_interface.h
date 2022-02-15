#ifndef MAIN_INTERFACE_H
#define MAIN_INTERFACE_H

#include <QWidget>
#include "Widget/main_in.h"
#include "Widget/main_out.h"
#include "Widget/main_inmanage.h"
#include "Widget/set.h"
#include "Widget/main_vehicl_equery.h"
#include "Widget/main_playback.h"
#include<QTimer>
namespace Ui {
class Main_Interface;
}

class Main_Interface : public QWidget
{
    Q_OBJECT

public:
    explicit Main_Interface(QWidget *parent = nullptr);
    ~Main_Interface();
    Main_In *inwin;
    Main_Out *outwin;
    Main_Inmanage *inmanagewin;
    Main_Vehicl_Equery *equerywin;
    Main_Playback *playbackwin;
    Set *setwin;
    void init_connect();

private slots:
    void qtimeSlot();

    void on_out_in_butt_clicked();

    void on_inside_butt_clicked();

    void on_set_butt_clicked();

    void on_query_butt_clicked();

    void on_playback_butt_clicked();

    void update_car_amount(int num);

private:
    Ui::Main_Interface *ui;
    int mark;
    QTimer *time;
};

#endif // MAIN_INTERFACE_H
