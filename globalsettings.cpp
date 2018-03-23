#include "globalsettings.h"
#include <QDebug>
#include<QDir>


GlobalSettings::GlobalSettings()
{
//    m_str_PayloadComm = "/dev/ttyUSB0";
//    m_uint_PayloadPortBaud = 115200;

//    m_str_FireCtrlComm = "/dev/ttyUSB1";
//    m_uint_FireCtrlPortBaud = 115200;

//    m_str_LocalAddr_IP = "192.168.10.200";
//    m_str_RemoteAddr_IP = "192.168.10.56";
//    m_uint_LocalAddr_Port = 20002;
//    m_uint_RemoteAddr_Port = 20000;

}

GlobalSettings::~GlobalSettings ()
{
    if(NULL != instance)
    {
        delete instance;
        instance = NULL;
    }
}

GlobalSettings* GlobalSettings::instance = NULL;
GlobalSettings* GlobalSettings::getInstance ()
{
    if(NULL == instance)
    {
        instance = new GlobalSettings();
    }
    return instance;
}

void GlobalSettings::initSettings ()
{
    QSettings settings("/home/nvidia/appsettings/Settings.ini",QSettings::IniFormat);

    m_str_PayloadComm = settings.value ("PayloadPort").toString ();
    m_uint_PayloadPortBaud = settings.value ("PayloadBaudrate").toInt ();

    m_str_FireCtrlComm = settings.value ("FireCtrlPort").toString ();
    m_uint_FireCtrlPortBaud = settings.value ("FireCtrlBaudrate").toInt ();

    m_str_LocalAddr_IP = settings.value ("LocalIPAddress").toString ();
    m_uint_LocalAddr_Port = settings.value ("LocalPort").toInt ();
    m_str_RemoteAddr_IP = settings.value ("RemoteIPAddress").toString ();
    m_uint_RemoteAddr_Port = settings.value ("RemotePort").toInt ();

#if 0
    QString strInitSettings;
    strInitSettings = "Current Working Dir:" + QDir::currentPath () +"\n\r"+ "Initializing global Settings";
    strInitSettings = strInitSettings + "Payload device:" + m_str_PayloadComm +"\n\r";
    strInitSettings = strInitSettings + "FireContrl device:" + m_str_FireCtrlComm;
    strInitSettings = strInitSettings + "Local IP:" + m_str_LocalAddr_IP ;
    strInitSettings = strInitSettings + "ADT IP:" + m_str_RemoteAddr_IP ;

    qDebug()<<strInitSettings;
#endif

}


quint16 GlobalSettings::CRC_check(QByteArray bufData)
{
    quint32 i,j;
    quint16 reg_CRC = 0xffff;
    if(bufData.isEmpty ())
    {
        return 0;
    }
    quint32 dataLen = bufData.length ();
    for(i = 0;i<dataLen;i++)
    {
        reg_CRC ^= bufData[i];
        for(j=0; j<8; j++)
        {
            if(reg_CRC & 0x01)
            {
                reg_CRC = (reg_CRC >> 1 ^ 0xa001);
            }
            else
            {
                reg_CRC = reg_CRC >> 1;
            }
        }
    }

    return reg_CRC;
}




















