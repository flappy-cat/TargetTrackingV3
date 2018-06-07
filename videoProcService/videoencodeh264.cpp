#include "videoencodeh264.h"
#include <unistd.h>
#include <QDebug>
#include "globalsettings.h"
#include "datamanager.h"
#include<limits.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define FIFO_NAME "/var/flappyfifo.tt"
#define FIFO_SIZE PIPE_BUF
VideoEncodeH264::VideoEncodeH264(QObject* parent):QThread(parent)
{

    framecnt = 0;
    capFrameCnt = 0;

    bufVideoSend.fill (0);
    bufVideoSend.resize (2048);
    rtspLoop=NULL;
}


VideoEncodeH264::~VideoEncodeH264 ()
{

}

void VideoEncodeH264::run()
{
    StartEncodeLoop ();
}



void VideoEncodeH264::StartEncodeLoop ()
{
    GlobalSettings *pGlobalSetting = GlobalSettings::getInstance ();
    DataManager *pDataManager = DataManager::GetInstance ();
    GlobalInit();
    while(1)
    {
        pDataManager->bIsStartVideoCapture = true;
        while(!videoSource.isOpened ())
        {

            if(pDataManager->videoSrc == 0x01)
            {

                videoSource.open ("v4l2src device=/dev/video1 ! \
'video/x-raw,format=(string)I420,width=1280,height=720,framerate=25/1' ! \
nvvidconv flip-method=0 ! \
video/x-raw, format=(string)BGRx ! \
videoconvert ! \
video/x-raw, format=(string)BGR ! appsinsk");
                if(videoSource.isOpened ())
                {
                    qDebug()<<"OPEN CCD SUCESS!";
                }
                else
                {
                    qDebug()<<"OPEN CCD FAILED!";
                }
/*
gst-launch-1.0 v4l2src device=/dev/video1 ! 'video/x-raw,format=(string)I420,width=1280,height=720,framerate=25/1' ! omxh265enc bitrate=512000 ! 'video/x-h265,stream-format=(string)byte-stream' ! h265parse ! filesink location=/var/flappyfifo
*/
videoSink.open("appsrc ! \
autovideoconvert ! \
omxh265enc bitrate=1000000 ! \
'video/x-h265,stream-format=(string)byte-stream' ! \
h265parse ! \
filesink location=/var/flappyfifo.tt sync=false async=false",0,(double)25,cv::Size(1280,720),true);
                        if(videoSink.isOpened ())
                        {
                            qDebug()<<"OPEN video sink SUCCESS!";
                        }
                        else
                        {
                            videoSource.release();
                            qDebug()<<"OPEN video sink FAILED!";
                        }
            }
            else if(pDataManager->videoSrc == 0x02)
            {
                videoSource.open ("nvcamerasrc  sensor_id=7 ! \
video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)I420, framerate=(fraction)25/1 ! \
nvvidconv flip-method=0 ! \
video/x-raw, format=(string)BGRx ! \
videoconvert ! \
video/x-raw, format=(string)BGR ! appsinsk");


                if(videoSource.isOpened ())
                {
                    qDebug()<<"OPEN INFRA CAM SUCESS!";
                }
                else
                {
                    qDebug()<<"OPEN INFRA CAM FAILED!";
                }
                        videoSink.open("appsrc ! \
                        autovideoconvert ! \
                        omxh265enc bitrate=1000000 ! \
                        'video/x-h265,stream-format=(string)byte-stream' ! \
                        h265parse ! \
                        filesink location=/var/flappyfifo sync=false async=false",0,(double)25,cv::Size(1280,720),true);
                        if(videoSink.isOpened ())
                        {
                            qDebug()<<"OPEN video sink SUCCESS!";
                        }
                        else
                        {
                        videoSource.release();
                            qDebug()<<"OPEN video sink FAILED!";
                        }
                                    }
            }
        }



        while(1)
        {
            if(!pDataManager->bIsStartVideoCapture)
            {
                videoSource.release ();
                capFrameCnt = 0;
                break;
            }

            videoSource >> capFrame;

            if(pDataManager->bIsStartTracking == true)
            {
                if(pDataManager->bIsResetTracking == true)
                {
                    //Visible reletive algorium enabled when visibleVideo is on and night vision is closed
                    if((pDataManager->videoSrc == 0x01) && (pDataManager->dayNightReverse == 0x55))
                    {
                            trackAlgorithm.MatchAndTrackInit (pDataManager->recvTargetRect,capFrame,pDataManager->featureVisible,50,1);

                            trackAlgorithm.ReadCalibrateFile (1, pDataManager->focusZoom);
                    }
                    else if((pDataManager->videoSrc == 0x01) && (pDataManager->dayNightReverse == 0xAA))
                    {
                        trackAlgorithm.MatchAndTrackInit (pDataManager->recvTargetRect,capFrame,pDataManager->featureInfrared,15,2);

                        trackAlgorithm.ReadCalibrateFile (1, pDataManager->focusZoom);
                    }
                    //and the others
                    else if(pDataManager->videoSrc == 0x02)
                    {
                        trackAlgorithm.MatchAndTrackInit (pDataManager->recvTargetRect,capFrame,pDataManager->featureInfrared,15,2);//2->1

                        trackAlgorithm.ReadCalibrateFile (2, pDataManager->electronicZoom);//2-->1
                    }
                    else
                    {

                    }
                    pDataManager->bIsResetTracking = false;
                }
                else
                {

                    Rect resultRect = trackAlgorithm.KeepTrack (capFrame);
                    rectangle (capFrame, resultRect, Scalar(255,0, 0, 0), 2, 8, 0);


                    Point pointIn = Point(resultRect.x+resultRect.width/2,
                                          resultRect.y+resultRect.height/2);

                    if((pDataManager->videoSrc == 0x01))// && (pDataManager->dayNightReverse == 0x55))
                    {

                        trackAlgorithm.Point2Angle (pointIn, pDataManager->yawAngle_target,
                                                    pDataManager->pitchAngle_target,1);
                    }
                    else// if(pDataManager->videoSrc == 0x02)
                    {

                        trackAlgorithm.Point2Angle (pointIn, pDataManager->yawAngle_target,
                                                    pDataManager->pitchAngle_target,2);
                    }



                    pDataManager->yawAngle_target = pDataManager->yawAngle_target*180/PI;
                    pDataManager->pitchAngle_target =pDataManager->pitchAngle_target*180/PI;

                    if(pDataManager->assistFlag == 0x01)//open
                    {

                        float yaw_pre,pitch_pre;
                        pDataManager->mutex_predictdata.lock ();
                        yaw_pre = pDataManager->yawAngle_predict;
                        pitch_pre = pDataManager->pitchAngle_predict;
                        pDataManager->mutex_predictdata.unlock ();

                        Point pointCross;
                        if((pDataManager->videoSrc == 0x01) && (pDataManager->dayNightReverse == 0x55))//pDataManager->videoSrc == 0x01)
                        {
                            trackAlgorithm.Angle2Point (yaw_pre,pitch_pre, pointCross, 1);
                        }
                        else// if(pDataManager->videoSrc == 0x02)
                        {
                            trackAlgorithm.Angle2Point (yaw_pre,pitch_pre, pointCross, 2);
                        }

                        //========Draw predict point ==============
                        //     |
                        //   --O--
                        //     |
                        //=========================================
                        static Scalar colorTargetTag(250,250,250);
                        {
                            int x = pointCross.x;
                            int y = pointCross.y;

                            line(capFrame,Point(x-40,y) , Point(x-10,y) ,colorTargetTag , 2 ,8 ,0);
                            line(capFrame,Point(x+10,y) , Point(x+40,y) ,colorTargetTag , 2 ,8 ,0);

                            line(capFrame,Point(x,y-40) , Point(x,y-10) ,colorTargetTag , 2 ,8 ,0);
                            line(capFrame,Point(x,y+10) , Point(x,y+40) ,colorTargetTag , 2 ,8 ,0);

                            circle (capFrame,Point(x,y) , 10 ,colorTargetTag , 1 ,LINE_AA ,0);


                        }

                    }

                }
            }
            if(pDataManager->assistFlag == 0x01)
              {
                //Set flag on image center
                int imgCenterX;
                int imgCenterY;

                if(pDataManager->focusZoom>=1 && pDataManager->focusZoom<=30)
                {
                    imgCenterX = (int)pDataManager->cameraMatrix[pDataManager->focusZoom -1].at<double>(0,2);//capFrame.cols / 2;
                    imgCenterY = (int)pDataManager->cameraMatrix[pDataManager->focusZoom -1].at<double>(1,2);//capFrame.rows / 2;
                }
                line(capFrame,Point(imgCenterX-20,imgCenterY) , Point(imgCenterX+20,imgCenterY) ,Scalar(250,250,250), 1 ,8 ,0);
                line(capFrame,Point(imgCenterX,imgCenterY-20) , Point(imgCenterX,imgCenterY+20) ,Scalar(250,250,250) , 1 ,8 ,0);
             }

            resize (capFrame, capFrame, Size(640, 480));
            videoSink.write(capFrame);
        }

            //usleep (5000);
            capFrameCnt++;
}






void VideoEncodeH264::GlobalInit()
{
//TODO: ADD CREATE FIFO FILE CODE HERE
    int fifo;
    unlink(FIFO_NAME);
    if(fifo = mkfifo(FIFO_NAME,O_WRONLY)<0)
    {
        qDebug()<<"mkfifo error";
    }
    if(fifo_fd=open(FIFO_NAME,O_RDONLY)<0)
    {
        qDebug()<<"open fifo error";
    }
rtspLoop = new QProcess(0);
QString exepath = "/usr/local/bin/testOnDemandRTSPServer";
QStringList paramlist;
rtspLoop->start (exepath,paramlist);

}
