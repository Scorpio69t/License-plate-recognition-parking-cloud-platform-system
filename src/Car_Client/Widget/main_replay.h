#ifndef MAIN_REPLAY_H
#define MAIN_REPLAY_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTimer>
#include <iostream>
using namespace std;


namespace Ui {
class Main_Replay;
}

class Main_Replay : public QWidget
{
    Q_OBJECT

public:
    explicit Main_Replay(QString path,QWidget *parent = nullptr);
    ~Main_Replay();
    void init_connect();

    void open_video();


private:
    Ui::Main_Replay *ui;
    QImage image;
    QTimer UpdatePlayTime;

    int LoadVideo(QString path);

    void ReadPlay();
    void Playing();
    void PausePlay();

    bool videoTimeSilderPressed = false;
    bool playing = false;

private slots:

    void receiveImg(QImage img);

    void UpdatePlayTimeOut();

    void on_BT_PlayOrPause_clicked();

    void on_videoTimeSilder_sliderPressed();

    void on_videoTimeSilder_sliderReleased();

    void on_comboBox_currentIndexChanged(int index);

protected:
        void paintEvent(QPaintEvent *);
};

#endif // MAIN_REPLAY_H
