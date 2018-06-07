#ifndef VIDEOENCODEH264_H
#define VIDEOENCODEH264_H

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/core/core.hpp"

using namespace cv;

#include <QHostAddress>

#include <QObject>
#include "trackalgorithm.h"
#include "QTime"
#include <QThread>
#include<QProcess>
class VideoEncodeH264 : public QThread
{
    Q_OBJECT
public:
    VideoEncodeH264(QObject* parent = 0);
    ~VideoEncodeH264();
protected:
    void run();
public:
    void StartEncodeLoop();

public:
    //--------函数声明------//
    void GlobalInit();


    VideoCapture videoSource;
    VideoWriter videoSink;
    TrackAlgorithm trackAlgorithm;

    //--------变量声明-----------//

    Mat capFrame;
    Mat resizeFrame;

    int64_t framecnt;
    int64_t capFrameCnt;

    int ret, got_output;

    QByteArray bufVideoSend;
    //=======For evaluating the time
    QTime m_timeStart;
    QTime m_timeEnd;

    int fifo_fd;//named pipe file descriptor
    QProcess* rtspLoop;
};

#endif // VIDEOENCODEH264_H
