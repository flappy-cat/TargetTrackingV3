#include "udpsendservice.h"
#include <QHostAddress>


UdpSendService::UdpSendService(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);

}


UdpSendService::~UdpSendService()
{
    if(instance != NULL)
    {
        delete instance;
        instance = NULL;
    }
}

UdpSendService* UdpSendService::instance = NULL;

UdpSendService* UdpSendService::GetInstance()
{
    if(NULL == instance)
    {
        instance = new UdpSendService();
    }

    return instance;
}














