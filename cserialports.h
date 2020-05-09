#ifndef CSERIALPORTS_H
#define CSERIALPORTS_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>
#include <QList>
#include <QMutex>

typedef enum _ERROR_CODE {
    _ERROR_CODE_SUCC = 0,
    _ERROR_CODE_FAIL,
    _ERROR_CODE_NOTFIND,
    _ERROR_CODE_FIND_SUCC,
    _ERROR_CODE_OPEN_SUCC,
    _ERROR_CODE_OPEN_FAIL,
    _ERROR_CODE_CLOSE_SUCC,
    _ERROR_CODE_CLOSE_FAIL,
    _ERROR_CODE_CONNET_FAIL,
    _ERROR_CODE_CONNET_SUCC,
    _ERROR_CODE_READ_FAIL,
    _ERROR_CODE_READ_SUCC,
    _ERROR_CODE_TIMEOUT
} ERROR_CODE;

typedef enum _THREAD_STATUS{
    _THREAD_STATUS_WORKING,
    _THREAD_STATUS_PAUSE,
    _THREAD_STATUS_STOP,
} THREAD_STATUS;

class CSerialPorts : public QObject
{
    Q_OBJECT
public:
    CSerialPorts(void);
    ~CSerialPorts(void);

private:
    bool m_isPrintingInfo;
    THREAD_STATUS m_threadStatus;
    QSerialPort::BaudRate m_baudrate;
    QList<QSerialPort*> m_deviceports;
    QList<QSerialPortInfo> m_ports_info;
    QByteArray m_dataToWrite;
    QStringList m_ports_list;
    QMutex m_mutex_threadStaus;
	QMutex m_mutex_PrintStatus;
    QMutex m_mutex_dataToWrite;

    ERROR_CODE (*m_handleDataFun)(QByteArray data);

private:
    void findDevices(void); // find all tags and BSs
    ERROR_CODE openDevice(QSerialPort* device);
    ERROR_CODE closeDevice(QSerialPort &device);
    QByteArray readData(QSerialPort* device);
    void writeData(QSerialPort* device, const QByteArray &data);	

    void setThreadStatus(THREAD_STATUS &status);

public:
    int getDeviceNum(void);
	
    void switchPrintOnOff(void);
	
    ERROR_CODE initialize(void);
    ERROR_CODE pauseSerialPort(void);
    ERROR_CODE continueSerialPort(void);
    ERROR_CODE stopSerialPort(void);

    void setDataToSend(QByteArray data);

    void setHandleDataFun(ERROR_CODE (*handleDataFun)(QByteArray data));

public slots:
    void doWorks(void);

signals:
    void workReady(void);
};

#endif // CSERIALPORTS_H
