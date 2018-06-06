#include <QCoreApplication>
#include "globalsettings.h"
#include "serviceudpvideo.h"
#include "serviceserialpayload.h"
#include "managerofcommunicaion.h"
#include "serviceserialfirectrl.h"

#include "videoencodeh264.h"
#include "udpsendservice.h"
int main(int argc, char *argv[])
{
    QCoreApplication    a(argc, argv);

    //1-Initialize the Global Settings
    GlobalSettings* pInstance = GlobalSettings::getInstance ();
    pInstance->initSettings ();

    UdpSendService* pSender = UdpSendService::GetInstance ();
    Q_UNUSED(pSender);

    //===============Get the singleton
    ServiceSerialPayload*   pSerialServicePayload   =      ServiceSerialPayload::getInstance ();
    ServiceSerialFireCtrl*  pSerialServiceFireCtrl  =      ServiceSerialFireCtrl::getInstance ();

    //3-Start the SerialPort Service
    pSerialServicePayload->StartService ();
    pSerialServiceFireCtrl->StartService ();


    VideoEncodeH264     encThread;
    ServiceUDPVideo     udpThread;

    QObject::connect (&encThread,SIGNAL(finished()),&encThread,SLOT(deleteLater()));
    QObject::connect (&udpThread,SIGNAL(Send_Data_signal(QByteArray)),pSerialServicePayload,SLOT(Send_Data(QByteArray)));

    qDebug()<<"Starting Initialize Global Serttings\n";

    //4-Start VideoCapture

    encThread.start ();

    return a.exec();
}
/*
TODO list:
20180606 pm:
1.add fifo file create code
2.execute rtsp automatically.

*/
