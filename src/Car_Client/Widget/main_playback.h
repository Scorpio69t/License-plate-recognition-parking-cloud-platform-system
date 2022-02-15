#ifndef MAIN_PLAYBACK_H
#define MAIN_PLAYBACK_H

#include <QWidget>
#include <QListWidgetItem>
#define DATE 0
#define DAY  1
namespace Ui {
class Main_Playback;
}

class Main_Playback : public QWidget
{
    Q_OBJECT

public:
    explicit Main_Playback(QWidget *parent = nullptr);
    ~Main_Playback();
    void dateOrday();
    void init_connect();
    int show_video();


private slots:
    void on_month_butt_toggled(bool checked);

    void on_day_butt_toggled(bool checked);

    void click_date(QListWidgetItem *);

    void click_video(QListWidgetItem *);

    void on_previous_butt_clicked();

    void on_next_butt_clicked();

private:
    Ui::Main_Playback *ui;
    int row, col;
    int state;
    int num;
    QString video_date;

};

#endif // MAIN_PLAYBACK_H
