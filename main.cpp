#include <QCoreApplication>
#include "globalsettings.h"
#include "serviceudpvideo.h"
#include "serviceserialpayload.h"
#include "managerofcommunicaion.h"
#include "serviceserialfirectrl.h"
#include "log4cat.h"
#include "videoencodeh264.h"
int main(int argc, char *argv[])
{
    QCoreApplication    a(argc, argv);

    //1-Initialize the Global Settings
    GlobalSettings* pInstance = GlobalSettings::getInstance ();
    pInstance->initSettings ();


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

    Log4Cat * pLog =    Log4Cat::getInstance ();
    pLog->InitLog ("/home/nvidia/appsettings/log4cplus.properties");
    qDebug()<<"Starting Initialize Global Serttings\n";




    //4-Start VideoCapture

    encThread.start ();

    return a.exec();
}

