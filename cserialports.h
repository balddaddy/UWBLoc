#ifndef CSERIALPORTS_H
#define CSERIALPORTS_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>
#include <QList>
#include "public.h"
#include "cprocrawdata.h"

class CSerialPorts : public QObject
{
    Q_OBJECT
public:
    CSerialPorts(void);
    ~CSerialPorts(void);

private:
    THREAD_STATUS m_threadStatus;
    QSerialPort::BaudRate m_baudrate;
    QList<QSerialPort*> m_deviceports;
    QList<QSerialPortInfo> m_ports_info;
    QStringList m_ports_list;
    QByteArray m_dataToWrite;
    QMutex m_mutex_threadStaus;
    QMutex m_mutex_dataToWrite;

    ERROR_CODE (*m_handleDataFun)(QByteArray, cProcRawData*);
    cProcRawData* m_procDataDevice;

private:
    void findDevices(void); // find all tags and BSs
    ERROR_CODE openDevice(QSerialPort* device);
    ERROR_CODE closeDevice(QSerialPort &device);
    QByteArray readData(QSerialPort* device);
    void writeData(QSerialPort* device, const QByteArray &data);	

    void setThreadStatus(THREAD_STATUS &status);
    THREAD_STATUS getThreadStatus(void);

public:
    int getDeviceNum(void);
	
    ERROR_CODE initialize(void);
    ERROR_CODE pauseSerialPort(void);
    ERROR_CODE continueSerialPort(void);
    ERROR_CODE stopSerialPort(void);

    void setDataToSend(QByteArray data);

    void setHandleDataFun(ERROR_CODE (*handleDataFun)(QByteArray, cProcRawData*), cProcRawData* device);

public slots:
    void doWorks(void);

signals:
    void workReady(void);
};

#endif // CSERIALPORTS_H
