#include "thread_out.h"

Thread_Out::Thread_Out()
{
    this->capture.open("./video.mp4");
    this->cascade.load("./data/cars.xml");
}

void Thread_Out::detectCarDraw(Mat &img,CascadeClassifier &cascade,double scale)
{
    //我们使用级联分类器前有一套标准处理流程，这个处理流程目的是加快计算机识别物体的速度。
    //对于原始视频来说，每张图片都非常大
    //1.先转灰度
    vector<Rect> cars;
    Mat grayImg,smallImg(cvRound(img.rows/scale),cvRound(img.cols/scale),CV_8UC1);
    cvtColor(img,grayImg,CV_BGR2GRAY);
    //2.再把图片缩小一半
    resize(grayImg,smallImg,smallImg.size(),0,0);
    //3.叫直方图均值化(这个效果是增强图片黑白对比度
    equalizeHist(smallImg,smallImg);
    //4。才是真正的开始利用级联分类器进行识别物体
    // cars:表示通过级联分类器识别完成之后会把数据存放到cars中
    //1.1表示每次识别后图像矩形框扩大1.1
    //3.表示opencv识别物体的时候最少检测3次才是算是目标
    //4.Size(30,30)能识别的物体最小必须大于30*30像素。
    cascade.detectMultiScale(smallImg,cars,1.1,3,0|CV_HAAR_SCALE_IMAGE,Size(30,30));
    for (vector<Rect>::const_iterator r= cars.begin();r!=cars.end();r++)
    {
        //在原始图片中绘制，不是在smallimg中绘制
        rectangle(img,cvPoint(r->x*scale,r->y*scale),
                  cvPoint((r->x+r->width)*scale,(r->y+r->height)*scale),
        Scalar(0,255,0),3);
    }
}


void Thread_Out::run()
{
    while(capture.read(picture))
    {
        if(picture.data)
        {
            detectCarDraw(picture,cascade,2);

            cvtColor(picture,img_RGB,CV_BGR2RGB);
            img =QImage((const unsigned char*)img_RGB.data,img_RGB.cols,img_RGB.rows,QImage::Format_RGB888);
            emit sendImg(img);
            emit sendMat(picture);
            //msleep(24);
        }
    }
}
