#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#define FLAPPYDEBUG 1

#define SAVELOCALFILE 1

#define PERIOD_CTRL 40
#include <QSettings>
#include <QString>
#include <stdint.h>


class GlobalSettings
{
private:
    GlobalSettings();
    ~GlobalSettings();
private:
     static GlobalSettings* instance;
public:
     static GlobalSettings* getInstance();
public:
     QString m_str_PayloadComm;
     quint32 m_uint_PayloadPortBaud;

     QString m_str_FireCtrlComm;
     quint32 m_uint_FireCtrlPortBaud;

     QString m_str_LocalAddr_IP;
     quint16 m_uint_LocalAddr_Port;
     QString m_str_RemoteAddr_IP;
     quint16 m_uint_RemoteAddr_Port;
public:
     void initSettings();

     static quint16 CRC_check(QByteArray bufData);

};

#endif // GLOBALSETTINGS_H
