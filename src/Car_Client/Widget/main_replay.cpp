#include "main_replay.h"
#include "ui_main_replay.h"
#include "Ffmpeg/ffmpeg_decode.h"

Main_Replay::Main_Replay(QString path,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Main_Replay)
{
    ui->setupUi(this);
    ui->videoTimeSilder->setEnabled(true);
    ui->videoTimeSilder->setMaximum(Ffmpeg_Decode::getInstance()->GetTotalTimeMsec());
    UpdatePlayTime.start(100);

    init_connect();
    ReadPlay();
    LoadVideo(path);
    Playing();

}

Main_Replay::~Main_Replay()
{
    delete ui;
}

void Main_Replay::init_connect()
{
    connect(Ffmpeg_Decode::getInstance(),SIGNAL(sendImg(QImage)),this,SLOT(receiveImg(QImage)));
    connect(&UpdatePlayTime, SIGNAL(timeout()), this, SLOT(UpdatePlayTimeOut()));
}

void Main_Replay::PausePlay() {
    playing = false;
    ui->BT_PlayOrPause->setText(tr("播放"));
}


void Main_Replay::ReadPlay()
{

    playing = true;
    ui->videoTimeSilder->setEnabled(true);
    ui->BT_PlayOrPause->setEnabled(true);
    ui->BT_PlayOrPause->setText(tr("播放"));
    ui->videoTimeSilder->setValue(0);
    ui->LB_curTime->setText(QString("00:00"));
    ui->LB_totalTime->setText(QString("00:00"));

}

void Main_Replay::Playing()
{
    ui->videoTimeSilder->setEnabled(true);
    ui->BT_PlayOrPause->setText(tr("暂停"));
    ui->videoTimeSilder->setMaximum(Ffmpeg_Decode::getInstance()->GetTotalTimeMsec());
    int totalMsec = Ffmpeg_Decode::getInstance()->GetTotalTimeMsec();
    int totalSec = totalMsec / 1000;
    ui->LB_totalTime->setText(QString("%1:%2").arg(totalSec / 60, 2, 10,QLatin1Char('0')).arg(totalSec% 60, 2,10,QLatin1Char('0')));
}

int Main_Replay::LoadVideo(QString path)
{

    Ffmpeg_Decode::getInstance()->open_video(path);
    Ffmpeg_Decode::getInstance()->play();

    return 0;
}


void Main_Replay::UpdatePlayTimeOut()
{

    int curMsec = Ffmpeg_Decode::getInstance()->GetCurTimeMsec();

    if (videoTimeSilderPressed == false)
        ui->videoTimeSilder->setValue(curMsec);
        int curSec = curMsec / 1000;
    ui->LB_curTime->setText(QString("%1:%2").arg(curSec / 60, 2, 10, QLatin1Char('0')).arg(curSec% 60, 2, 10, QLatin1Char('0')));

}


//暂停和播放槽函数
void Main_Replay::on_BT_PlayOrPause_clicked()
{
    //当前如果是播放就暂停
    if (playing)
    {
        PausePlay();
        Ffmpeg_Decode::getInstance()->pause();
        UpdatePlayTime.stop();
    }
    else
    {
        playing = true;
        Playing();
        Ffmpeg_Decode::getInstance()->play();
        UpdatePlayTime.start(100);
    }
}

//滑动进度条鼠标按键按下
void Main_Replay::on_videoTimeSilder_sliderPressed()
{
    videoTimeSilderPressed = true;
}

//滑动进度条鼠标按键释放
void Main_Replay::on_videoTimeSilder_sliderReleased()
{
    UpdatePlayTime.stop();
    videoTimeSilderPressed = false;
    qDebug("rel pos:%d", ui->videoTimeSilder->value());

    Ffmpeg_Decode::getInstance()->Seek(ui->videoTimeSilder->value());
    PausePlay();
}


//接收图片函数
void Main_Replay::receiveImg(QImage img)
{
    this->image =img;
    this->update();
}

//监控界面label控件接收图片事件
void Main_Replay::paintEvent(QPaintEvent *)
{
    //如果这个图片不为空
    if(!this->image.isNull())
    {
        //显示在label上
        QPixmap *image = new QPixmap(QPixmap::fromImage(this->image));
        image->scaled(ui->video_label->size(),Qt::KeepAspectRatio);
        ui->video_label->setPixmap(QPixmap::fromImage(this->image));
        ui->video_label->setScaledContents(true);
    }
}


//视频播放速率控制槽函数
void Main_Replay::on_comboBox_currentIndexChanged(int index)
{
    if(index == 0)
    {
        Ffmpeg_Decode::getInstance()->change_play_speed(25);
    }
    else if(index == 1)
    {
        Ffmpeg_Decode::getInstance()->change_play_speed(50);
    }
    else if(index == 2)
    {
        Ffmpeg_Decode::getInstance()->change_play_speed(12);
    }
}
