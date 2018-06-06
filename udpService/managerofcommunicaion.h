#ifndef MANAGEROFCOMMUNICAION_H
#define MANAGEROFCOMMUNICAION_H

#include <QObject>
#include "serviceudpvideo.h"
#include <QThread>


class ManagerOfCommunicaion : public QObject
{
    Q_OBJECT
public:
    explicit ManagerOfCommunicaion(QObject *parent = 0);
    ~ManagerOfCommunicaion();
private:
    static ManagerOfCommunicaion* instance;
public:
    static ManagerOfCommunicaion* getInstance();
public:
    void StartCommunication();
signals:

public slots:

private:
    QThread *pUdpSingleServiceThread;
    ServiceUDPVideo *pUDPSingleService;

};

#endif // MANAGEROFCOMMUNICAION_H
